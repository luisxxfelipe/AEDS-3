#include <stdio.h>
#include <stdlib.h>

typedef struct grafo
{
    int ponderado;
    int n_vertices;
    int grau_max;
    int **arestas;
    float **pesos;
    int *grau;
} s_grafo;

s_grafo *cria_Grafo(int n_vertices, int grau_max, int ponderado);
void libera_Grafo(s_grafo *grafo);
int insereAresta(s_grafo *grafo, int origem, int destino, int digrafo, float peso);
int remover_aresta(s_grafo *grafo, int origem, int destino, int digrafo);

void buscaProfundidade(s_grafo *grafo, int ini, int *visitado, int cont);
void buscaProfundidade_Grafo(s_grafo *grafo, int ini, int *visitado);
void buscaLargura_Grafo(s_grafo *grafo, int ini, int *visitado);
void menorCaminho_Grafo(s_grafo *grafo, int ini, int *antecessor, float *distancia);
void imprime_Grafo(s_grafo *grafo);

int main()
{
    int opcao;
    int origem, destino, digrafo = 1, qtd_vertices = 0, qtd_arestas = 0, grau;
    int i, anterior[5];
    float distancia[5];

    printf("\nInsira a quantidade de vertices (seguido de espaco) e a quantidade de arestas.\n");
    scanf("%d %d", &qtd_vertices, &qtd_arestas);

    s_grafo *grafo = cria_Grafo(qtd_vertices, qtd_arestas, 0);

    while (opcao != 4)
    {
        printf("\n\n[1]Adicionar elementos no grafo.\n[2]Buscar em largura no grafo.\n[3]Imprimir os elmentos do grafo.\n\n");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            printf("\nDigite a origem (seguido de espaco), o destino (seguido de espaco) e o grau: ");
            scanf("%d %d %d", &origem, &destino, &grau);
            insereAresta(grafo, origem, destino, digrafo, grau);
        }

        if (opcao == 2)
        {
            int visitar[5];
            int elemento;

            printf("Insira a partir de qual elemento deseja buscar: \n");
            scanf("%d", &elemento);
            buscaLargura_Grafo(grafo, elemento, visitar);

            menorCaminho_Grafo(grafo, elemento, anterior, distancia);

            for (i = 0; i < qtd_vertices; i++)
               printf("\nVertice %d: %d -> Distancia: %.2f.\n", i, anterior[i], distancia[i]);
        }

        if (opcao == 3)
        {
            imprime_Grafo(grafo);
        }
    }

    libera_Grafo(grafo);

    return 0;
}

s_grafo *cria_Grafo(int n_vertices, int grau_max, int ponderado)
{
    s_grafo *grafo;
    grafo = (s_grafo *)malloc(sizeof(struct grafo));
    if (grafo != NULL)
    {
        int i;
        grafo->n_vertices = n_vertices;
        grafo->grau_max = grau_max;
        grafo->ponderado = (ponderado != 0) ? 1 : 0;
        grafo->grau = (int *)calloc(n_vertices, sizeof(int));

        grafo->arestas = (int **)malloc(n_vertices * sizeof(int *));
        for (i = 0; i < n_vertices; i++)
            grafo->arestas[i] = (int *)malloc(grau_max * sizeof(int));

        if (grafo->ponderado)
        {
            grafo->pesos = (float **)malloc(n_vertices * sizeof(float *));
            for (i = 0; i < n_vertices; i++)
                grafo->pesos[i] = (float *)malloc(grau_max * sizeof(float));
        }
    }
    return grafo;
}

void libera_Grafo(s_grafo *grafo)
{
    if (grafo != NULL)
    {
        int i;
        for (i = 0; i < grafo->n_vertices; i++)
            free(grafo->arestas[i]);
        free(grafo->arestas);

        if (grafo->ponderado)
        {
            for (i = 0; i < grafo->n_vertices; i++)
                free(grafo->pesos[i]);
            free(grafo->pesos);
        }
        free(grafo->grau);
        free(grafo);
    }
}

int insereAresta(s_grafo *grafo, int origem, int destino, int digrafo, float peso)
{
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices)
        return 0;
    if (destino < 0 || destino >= grafo->n_vertices)
        return 0;

    grafo->arestas[origem][grafo->grau[origem]] = destino;
    if (grafo->ponderado)
        grafo->pesos[origem][grafo->grau[origem]] = peso;
    grafo->grau[origem]++;

    if (digrafo == 0)
        insereAresta(grafo, destino, origem, 1, peso);
    return 1;
}

int remover_aresta(s_grafo *grafo, int origem, int destino, int digrafo)
{
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices)
        return 0;
    if (destino < 0 || destino >= grafo->n_vertices)
        return 0;

    int i = 0;
    while (i < grafo->grau[origem] && grafo->arestas[origem][i] != destino)
        i++;
    if (i == grafo->grau[origem]) // elemento nao encontrado
        return 0;
    grafo->grau[origem]--;
    grafo->arestas[origem][i] = grafo->arestas[origem][grafo->grau[origem]];
    if (grafo->ponderado)
        grafo->pesos[origem][i] = grafo->pesos[origem][grafo->grau[origem]];
    if (digrafo == 0)
        remover_aresta(grafo, destino, origem, 1);
    return 1;
}

void imprime_Grafo(s_grafo *grafo)
{
    if (grafo == NULL)
        return;

    int i, j;
    for (i = 0; i < grafo->n_vertices; i++)
    {
        printf("%d: ", i);
        for (j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->ponderado)
                printf("A (%d)::Peso (%.2f), ", grafo->arestas[i][j], grafo->pesos[i][j]);
            else
                printf("A (%d) -> ", grafo->arestas[i][j]);
        }
        printf("\n");
    }
}

int procuraMenorDistancia(float *dist, int *visitado, int NV)
{
    int i, menor = -1, primeiro = 1;
    for (i = 0; i < NV; i++)
    {
        if (dist[i] >= 0 && visitado[i] == 0)
        {
            if (primeiro)
            {
                menor = i;
                primeiro = 0;
            }
            else
            {
                if (dist[menor] > dist[i])
                    menor = i;
            }
        }
    }
    return menor;
}

void menorCaminho_Grafo(s_grafo *grafo, int ini, int *ant, float *dist)
{
    int i, cont, NV, ind, *visitado, vert;
    cont = NV = grafo->n_vertices;
    visitado = (int *)malloc(NV * sizeof(int));
    for (i = 0; i < NV; i++)
    {
        ant[i] = -1;
        dist[i] = -1;
        visitado[i] = 0;
    }
    dist[ini] = 0;
    while (cont > 0)
    {
        vert = procuraMenorDistancia(dist, visitado, NV);
        // printf("u = %d\n",u);
        if (vert == -1)
            break;

        visitado[vert] = 1;
        cont--;
        for (i = 0; i < grafo->grau[vert]; i++)
        {
            ind = grafo->arestas[vert][i];
            if (dist[ind] < 0)
            {
                dist[ind] = dist[vert] + 1; // ou peso da aresta
                ant[ind] = vert;
            }
            else
            {
                if (dist[ind] > dist[vert] + 1)
                {
                    dist[ind] = dist[vert] + 1; // ou peso da aresta
                    ant[ind] = vert;
                }
            }
        }
    }

    free(visitado);
}

void buscaProfundidade(s_grafo *grafo, int ini, int *visitado, int cont)
{
    int i;
    visitado[ini] = cont;
    for (i = 0; i < grafo->grau[ini]; i++)
    {
        if (!visitado[grafo->arestas[ini][i]])
            buscaProfundidade(grafo, grafo->arestas[ini][i], visitado, cont + 1);
    }
}

void buscaProfundidade_Grafo(s_grafo *grafo, int ini, int *visitado)
{
    int i, cont = 1;
    for (i = 0; i < grafo->n_vertices; i++)
        visitado[i] = 0;
    buscaProfundidade(grafo, ini, visitado, cont);

    for (i = 0; i < grafo->n_vertices; i++)
        printf("\nElemento atual: %d -> Elemento visitado: %d\n", i, visitado[i]);
}

void buscaLargura_Grafo(s_grafo *grafo, int ini, int *visitado)
{
    int i, vert, NV, cont = 1;
    int *fila, IF = 0, FF = 0;
    for (i = 0; i < grafo->n_vertices; i++)
        visitado[i] = 0;

    NV = grafo->n_vertices;
    fila = (int *)malloc(NV * sizeof(int));
    FF++;
    fila[FF] = ini;
    visitado[ini] = cont;
    while (IF != FF)
    {
        IF = (IF + 1) % NV;
        vert = fila[IF];
        cont++;
        for (i = 0; i < grafo->grau[vert]; i++)
        {
            if (!visitado[grafo->arestas[vert][i]])
            {
                FF = (FF + 1) % NV;
                fila[FF] = grafo->arestas[vert][i];
                visitado[grafo->arestas[vert][i]] = cont;
            }
        }
    }
    free(fila);
    for (i = 0; i < grafo->n_vertices; i++)
        printf("\nElemento atual: %d -> Elemento visitado: %d\n", i, visitado[i]);
}
