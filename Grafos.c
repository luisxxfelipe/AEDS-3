#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define BRANCO 0
#define CINZA 1
#define PRETO 2

typedef struct Grafo
{
    int ponderado;   // confirma se o grafo é ponderado
    int n_vertices;  // numero de vertices de um grafo
    int grau_maximo; // grau maximo de arestas para um vertice
    int **arestas;   // vai guardar as conexoes, uma matriz alocada dinamicamente
    float **pesos;   // grafico ponderado
    int *grau;       // para cada vertice quantas arestas ele possui
} s_grafo;

s_grafo *criar_grafo(int n_vertices, int grau_maximo, int ponderado);
void libera_grafo(s_grafo *grafo);
int inserir_aresta(s_grafo *grafo, int origem, int destino, int digrafo, float peso);
int remover_aresta(s_grafo *grafo, int origem, int destino, int digrafo);
void imprimir_grafo(s_grafo *grafo);

int main()
{
    int digrafo = 1, opcao, origem, destino, vertices, arestas;

    printf("Defina o numeros de vertices - com um valor acima (seguido de espaco) - numero maximo de arestas para um vertice: ");
    scanf("%d %d", &vertices, &arestas);
    s_grafo *grafo = criar_grafo(vertices, arestas, 0);

    do
    {
        printf("\n[1]Adicionar elementos no grafo.\n[2]Remover elementos do grafo.\n[3]Imprimir os elmentos do grafo.\n\n");
        scanf("%d", &opcao);
        if (opcao == 1)
        {
            printf("\nDigite a origem (seguido de espaco), o destino: ");
            scanf("%d %d", &origem, &destino);
            inserir_aresta(grafo, origem, destino, digrafo, 0);
        }

        /*if (opcao == 2)
        {
            printf("\nDigite o elemento que deseja remover: ");
            scanf("%d", &remover);

            remover_aresta(grafo, origem, destino, digrafo, peso);
        }*/

        if (opcao == 3)
        {
            imprimir_grafo(grafo);
        }

    } while (opcao != 4);

    libera_grafo(grafo);

    return 0;
}

s_grafo *criar_grafo(int n_vertices, int grau_maximo, int ponderado) // funcao para criar o grafo, de acordo com seus vertices, arestas e grau
{
    s_grafo *grafo = (s_grafo *)malloc(sizeof(struct Grafo));
    if (grafo != NULL)
    {
        grafo->n_vertices = n_vertices;
        grafo->grau_maximo = grau_maximo;
        grafo->ponderado = (ponderado != 0) ? 1 : 0;          // verificacao para que o valor seja sempre 0 ou 1
        grafo->grau = (int *)calloc(n_vertices, sizeof(int)); // inicialmente cada vertice tem 0
        grafo->arestas = (int **)malloc(n_vertices * sizeof(int *));

        for (int i = 0; i < n_vertices; i++)
            grafo->arestas[i] = (int *)malloc(grau_maximo * sizeof(int)); // para cada vertice, to criando um grau maximo para ele (quantas areas ele pode ter)
        if (grafo->ponderado)                                             // quando o grafo for ponderado, cria outra lista de adjancencia (matriz), onde cada posicao vai armazenar o peso
        {
            grafo->pesos = (float **)malloc(n_vertices * sizeof(float *));
            for (int i = 0; i < n_vertices; i++)
                grafo->pesos[i] = (float *)malloc(grau_maximo * sizeof(float));
        }
    }

    return grafo;
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
    printf("\nO elemento foi inserido no grafo com sucesso.\n");
    return 1;
}

int remover_aresta(s_grafo *grafo, int origem, int destino, int digrafo) // funcao para remover os elementos de acordo com seu grau e aresta
{
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices) // vetor nao pode ser menor que 0 e nao pode ser maior que o numero de vertices
        return 0;
    if (destino < 0 || destino >= grafo->n_vertices)
        return 0;

    int i = 0;

    while (i < grafo->grau[origem] && grafo->arestas[origem][i] != destino) // vai procurar dentro da quantidade de arestas no vertice de origem, se existe a posicao i
    {                                                                       // enquanto for diferente, vai avançando nas arestas
        i++;

        if (i == grafo->grau[origem])
            return 0;

        grafo->grau[origem]--;                                                   // diminui a quantidade de graus do vertice origem
        grafo->arestas[origem][i] = grafo->arestas[origem][grafo->grau[origem]]; // na posicao i coloca a ultima aresta, ou seja, pega o ultimo elemento do vetor e coloca no lugar da origem

        if (grafo->ponderado)
            grafo->pesos[origem][i] = grafo->pesos[origem][grafo->grau[origem]];

        if (digrafo == 0)                              // se nao for digrafo
            remover_aresta(grafo, destino, origem, 1); // torna-se digrafo
        return 1;
    }
}

void imprimir_grafo(s_grafo *grafo) // funcao para imprimir os elementos do grafo
{
    if (grafo != NULL)
    {
        int i, j;
        for (i = 0; i < grafo->n_vertices; i++)
        {
            printf("%d: ", i);
            for (j = 0; j < grafo->grau[i]; j++)
            {
                if (grafo->ponderado)
                    printf("%d(%.2f", grafo->arestas[i][j], grafo->pesos[i][j]);
                else
                    printf("%d -> ", grafo->arestas[i][j]);
            }
            printf("\n");
        }
    }
}

void busca_profundida(s_grafo *grafo)
{
}

void visita_profundida(s_grafo *grafo)
{

}