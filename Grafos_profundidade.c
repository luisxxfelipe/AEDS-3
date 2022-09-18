#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0
typedef int bool;
typedef int grau;

#define BRANCO 0
#define CINZA 2
#define PRETO 3

typedef struct adjacencia
{
    int vertices;            // representado pelo vertice de destino
    int grau;                // representado pela quantidade de areas que ele possui
    struct adjacencia *prox; // representa o proximo elemento da lista de adjacencia
} s_adjacencia;

typedef struct vertice
{
    s_adjacencia *comeco; // representado pelo comeco da lista de adjacencia
} s_vertice;

typedef struct grafo
{
    int n_vertice;        // representa o numero de vertices do grafo
    int n_arestas;        // representa o numero de arestas do grafo
    s_vertice *adjacente; // representa o arranjo de vertices da estrutura
} s_grafo;

s_grafo *criar_grafo(int vertices);
s_adjacencia *criar_adjacencia(int vertices, int grau);
bool criar_aresta(s_grafo *grafo, int origem, int destino, grau g);
void imprimir_grafo(s_grafo *grafo);
void profundidade(s_grafo *grafo);
void visita_profundidade(s_grafo *grafo, int i, int *cor);

int main()
{
    int vertices, opcao = 0, origem, destino, grau;
    printf("\nDigite o numero de vertices que deseja que o grafo tenha: ");
    scanf("%d", &vertices);

    s_grafo *grafo = criar_grafo(vertices);

    while (opcao != 4)
    {
        printf("\n\n[1]Adicionar elementos no grafo.\n[2]Buscar algum elemento do grafo.\n[3]Imprimir os elmentos do grafo.\n\n");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            printf("\nDigite a origem (seguido de espaco), o destino (seguido de espaco), grau: ");
            scanf("%d %d %d", &origem, &destino, &grau);

            criar_aresta(grafo, origem, destino, grau);
        }

        if (opcao == 2)
        {
            int elemento[vertices];
            visita_profundidade(grafo, 0, elemento);
        }

        if (opcao == 3)
        {
            imprimir_grafo(grafo);
        }
    }

    return 0;
}

s_grafo *criar_grafo(int vertices)
{
    s_grafo *grafo = (s_grafo *)malloc(sizeof(s_grafo));                  // aloca espaço para a estrutura do grafo
    grafo->n_vertice = vertices;                                          // atualza o numero de vertices
    grafo->n_arestas = 0;                                                 // atualizo o numero de arestas
    grafo->adjacente = (s_vertice *)malloc(vertices * sizeof(s_vertice)); // alocando o arranjo com o vertice desejado

    for (int i = 0; i < vertices; i++)
    {
        grafo->adjacente[i].comeco = NULL; // iniciando como NULL todas as arestas
    }

    return grafo;
}

s_adjacencia *criar_adjacencia(int vertices, int grau) // funcao para adicionar arestas no grafo
{
    s_adjacencia *adj = (s_adjacencia *)malloc(sizeof(s_adjacencia)); // aloca espaço
    adj->vertices = vertices;                                         // vertice de destino da adjacencia
    adj->grau = grau;                                                 // representa o grau da aresta
    adj->prox = NULL;                                                 // aponta para NULL a proxima aresta
    return adj;                                                       // retorna ao endereço da adjacencia
}

bool criar_aresta(s_grafo *grafo, int origem, int destino, grau g) // funcao para representar para onde um vertice de origem liga a um vertice de destino
{
    if (!grafo) // se o grafo nao existe, retorna falso
        return false;
    if ((destino < 0) || (destino >= grafo->n_vertice)) // se o valor der negativo
        return false;
    if ((origem < 0) || (destino >= grafo->n_vertice)) // se o valor for superior ao numero de vertice do grafo
        return false;

    s_adjacencia *novo = criar_adjacencia(destino, g); // cria uma adjacencia com o vertice final e seu grau
    novo->prox = grafo->adjacente[destino].comeco;     // o proximo campo da adjacencia vai receber o começo da lista
    grafo->adjacente[origem].comeco = novo;            // e o comeco da lista passa a receber o novo elemento
    grafo->n_arestas++;                                // insere mais um numero de aresta no grafo
    return true;
}

void profundidade(s_grafo *grafo)
{
    int cor[grafo->n_vertice];

    for (int i = 0; i < grafo->n_vertice; i++)
    {
        cor[i] = BRANCO; // define que inicialmente todos sao brancoa
    }

    for (int i = 0; i < grafo->n_vertice; i++)
    {
        if (cor[i] == BRANCO) // se a cor for branca, ira visitar todos em profundidade
        {
            visita_profundidade(grafo, i, cor); // visita ate o final do grafo e retorna
        }
    }
}

void visita_profundidade(s_grafo *grafo, int i, int *cor)
{
    cor[i] = CINZA;                                     // visitar um no marcado de cinza
    s_adjacencia *vertice = grafo->adjacente[i].comeco; // visita a adjacencia

    while (vertice) // visita a adjacencia, aresta [i] e [vertice] recursivamente
    {
        if (cor[vertice->vertices] == BRANCO) // se a cor for branca, ira visitar
            visita_profundidade(grafo, i, cor);
        vertice = vertice->prox;
    }

    cor[i] = PRETO;

    for (i = 0; i < grafo->n_vertice; i++)
        printf("%d -> %d\n", i, cor[i]);
}

void imprimir_grafo(s_grafo *grafo)
{
    printf("Vertices: %d. Arestas: %d.\n", grafo->n_vertice, grafo->n_arestas); // imprime o numero de vertices e de arestas do grafo

    for (int i = 0; i < grafo->n_vertice; i++)
    {
        printf("\nA[%d] --> ", i);                      // imprime qual aresta esta
        s_adjacencia *adj = grafo->adjacente[i].comeco; // chama o comeco da lista de adjacencia dessa aresta
        while (adj)                                     // enquanto o numero de adjacencias nao for nulo
        {
            printf("V[%d] :: G(%d) ", adj->vertices, adj->grau); // imprime a adjacencia e o seu peso
            adj = adj->prox;
        }
    }
}
