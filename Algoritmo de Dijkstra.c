#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INFINITO 99999
#define MAX 15

typedef struct Grafo
{
    int distancia;
    int ponderado, n_vertices;                  // orientação e numero de vertices de um grafo
    int **arestas, **pesos;                     // vai guardar as conexoes, uma matriz alocada dinamicamente
    int *grau, grau_maximo;                     // para cada vertice quantas arestas ele possui
    int cor, tempo, tempo_inicial, tempo_final; // define a cor do no e o tempo inicial e final de busca
} s_grafo;

s_grafo *criar_grafo(int n_vertices, int grau_maximo, int ponderado);
int inserir_aresta(s_grafo *grafo, int origem, int destino, int digrafo, float peso);
void libera_grafo(s_grafo *grafo);
void imprimir_grafo(s_grafo *grafo);
void algoritmo_dijkstra(s_grafo *grafo, int origem);

int main()
{
    int opcao;
    int origem, destino, digrafo = 1, peso;
    int qtd_vert, grau_max;

    printf("Insira a quantidade de vertices e o grau maximo que deseja no seu grafo: \n");
    scanf("%d %d", &qtd_vert, &grau_max);

    s_grafo *grafo = criar_grafo(qtd_vert + 1, grau_max, 1);

    do
    {
        printf("\n[1]Inserir valores no grafo.\n[2]Imprimir o grafo.\n[3]Algoritmo de Dijkstra.\n");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            printf("\nDigite a origem (seguido de espaco), o destino e o seu peso: ");
            scanf("%d %d %d", &origem, &destino, &peso);
            inserir_aresta(grafo, origem, destino, digrafo, peso);
        }

        if (opcao == 2)
        {
            imprimir_grafo(grafo);
        }

        if (opcao == 3)
        {
            printf("\nIndique a origem e destino do grafo que deseja percorre: ");
            scanf("%d", &origem);

            algoritmo_dijkstra(grafo, origem);
        }
    } while (opcao != 4);
    libera_grafo(grafo);
}

s_grafo *criar_grafo(int n_vertices, int grau_maximo, int ponderado) // funcao para criar o grafo, de acordo com seus vertices, arestas e grau
{
    s_grafo *grafo = (s_grafo *)malloc(sizeof(struct Grafo));
    if (grafo != NULL)
    {
        grafo->n_vertices = n_vertices;
        grafo->grau_maximo = grau_maximo;
        grafo->ponderado = (ponderado != 0) ? 1 : 0;          // verificacao para que o valor seja sempre 0 ou 1
        grafo->grau = (int *)calloc(n_vertices, sizeof(int)); // inicialmente cada vertice tem grau 0
        grafo->arestas = (int **)malloc(n_vertices * sizeof(int *));

        for (int i = 0; i < n_vertices; i++)
            grafo->arestas[i] = (int *)malloc(grau_maximo * sizeof(int)); // para cada vertice, to criando um grau maximo para ele (quantas areas ele pode ter)
        if (grafo->ponderado)                                             // quando o grafo for ponderado, cria outra lista de adjancencia (matriz), onde cada posicao vai armazenar o peso
        {
            grafo->pesos = (int **)malloc(n_vertices * sizeof(int *));
            for (int i = 0; i < n_vertices; i++)
                grafo->pesos[i] = (int *)malloc(grau_maximo * sizeof(int));
        }
    }

    return grafo;
}

int inserir_aresta(s_grafo *grafo, int origem, int destino, int digrafo, float peso) // funcao para inserir no grafo os elementos, de acordo com seu peso
{
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices) // vetor nao pode ser menor que 0 e nao pode ser maior que o numero de vertices
        return 0;
    if (destino < 0 || destino >= grafo->n_vertices)
        return 0;

    grafo->arestas[origem][grafo->grau[origem]] = destino; // vertice inicial // e acrescenta o destino
    if (grafo->ponderado)                                  // se for ponderado ira adicionar o peso na matriz
        grafo->pesos[origem][grafo->grau[origem]] = peso;
    grafo->grau[origem]++; // soma mais um no grau do vertice de origem

    if (digrafo == 0) // se ele nao for digrafo
        inserir_aresta(grafo, destino, origem, 1, peso);
    // começa a ser digrafo
    // printf("\nO elemento foi inserido no grafo com sucesso.\n");
}

void libera_grafo(s_grafo *grafo) // funcao para liberar o grafo (free)
{
    if (grafo != NULL)
    {
        for (int i = 0; i < grafo->n_vertices; i++)

            free(grafo->arestas[i]); // libera as listas que foram criadas para as arestas
        free(grafo->arestas);        // por fim, libera o vetor das arestas

        if (grafo->ponderado)
        {
            for (int i = 0; i < grafo->n_vertices; i++)

                free(grafo->pesos[i]); // libera as listas que foram criadas para armazenar os pesos
            free(grafo->pesos);        // por fim, libera o vetor
        }

        free(grafo->grau); // elimina o vetor de grau
        free(grafo);
    }
}

void imprimir_grafo(s_grafo *grafo) // funcao para imprimir os elementos do grafo
{
    if (grafo == NULL)
        return;

    int i, j;
    for (i = 1; i < grafo->n_vertices; i++)
    {
        printf("V[%d]: ", i);
        for (j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->ponderado)
                printf("V(%d)::P(%d), ", grafo->arestas[i][j], grafo->pesos[i][j]);
            else
                printf("V(%d) -> ", grafo->arestas[i][j]);
        }
        printf("\n");
    }
}

void algoritmo_dijkstra(s_grafo *grafo, int origem)
{
    int custo[MAX][MAX], distancia[MAX], predecessor[MAX]; // foi criado uma matriz representando os custos, alem disso, criei um vetor para armazenar a distancia e o seu predecessor
    int visitado[MAX], contador, dist_minima, prox, i, j;  // criado um vetor para representar acaso o vertice foi visitado, um contador para representar o numero de vertices vistos, ...

    for (i = 0; i < grafo->n_vertices; i++)
    {
        for (j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->arestas[i][j] == 0)
                custo[i][j] = INFINITO;
            else
                custo[i][j] = grafo->arestas[i][j];
        }
    }

    for (i = 0; i < grafo->n_vertices; i++) // inicializando o custo, a distancia e o vetor de visita
    {
        distancia[i] = custo[origem][i];
        predecessor[i] = origem;
        visitado[i] = 0;
    }

    distancia[origem] = 0; // distancia de origem definida por 0
    visitado[origem] = 1;  // define o vertice de origem como 1
    contador = 1;          // contador de visita passa a ser 1

    while (contador < grafo->n_vertices - 1)
    {
        dist_minima = INFINITO; // define a distancia de todos os vertices como INFINITO
        // prox gives the node at minimum distancia
        for (i = 0; i < grafo->n_vertices; i++) // começo a visitar os proximos vertices, acaso a distancia for menor que a distancia minimima e que nao tenha sido visitado
            if (distancia[i] < dist_minima && !visitado[i])
            {
                dist_minima = distancia[i]; // distancia minima assume o valor da distancia
                prox = i;
            }

        visitado[prox] = 1;
        for (i = 0; i < grafo->n_vertices; i++) // verifica se existe um caminho melhor pelo laço e acaso tenha, predecessor passaa assumir a origem
            if (!visitado[i])
                if (dist_minima + custo[prox][i] < distancia[i])
                {
                    distancia[i] = dist_minima + custo[prox][i];
                    predecessor[i] = prox;
                }
        contador++;
    }

    for (i = 0; i < grafo->n_vertices; i++) // funcao para imprimir a distancia do vertice e seu caminho percorrido
        if (i != origem)
        {
            printf("\nDistancia do vertice (%d) e (%d)", i, distancia[i]);
            printf("\nCaminho percorrido: %d", i);
            j = i;
            do
            {
                j = predecessor[j];
                printf(" <-- %d", j);
            } while (j != origem);
        }
}