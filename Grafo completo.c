#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Grafo_elemento
{
    int chave;
    int distancia;
    struct Grafos *prox;
} grafo_elemento;

typedef struct Vertices
{
    int chave;
    grafo_elemento *prox;
} s_vertices;

typedef struct Grafos
{
    s_vertices *vertices;
    int numero_vertices;
} grafos;

int main()
{

    return 0;
}

grafos *criar_grafo(int tamanho) // funcao para criar o grafo (alocação) e inicializando os dados
{
    grafos *grafo;

    // alocando memoria para vertices e arestas * tamanho
    grafo = (grafos *)malloc(sizeof(grafos));
    grafo->vertices = (s_vertices *)malloc(sizeof(s_vertices) * tamanho);
    grafo->numero_vertices = 0; // inicializando o numero de vertices = 0

    return grafo;
}

int buscar_vertice(grafos *grafo, int chave, int tamanho)
{
    for (int i = 0; i < grafo->numero_vertices; i++)
        if (grafo->vertices[i].chave == chave)
            return i;

    return -1;
}


int inserir_vertice(grafos *grafo, int chave, int tamanho)
{
    if (grafo->numero_vertices < tamanho)
    {
        if (buscar_vertice(grafo, chave, tamanho) == -1)
        {
            grafo->vertices[grafo->numero_vertices].chave = chave;
            grafo->vertices[grafo->numero_vertices].prox = NULL;
            grafo->numero_vertices++;
            printf("\nO vertice [%d] foi inserido com sucesso.\n", chave);
            return 1;
        }
    }

    return 0;
}

void inserir_aresta(grafos *grafo, int chave_1, int chave_2, int distancia)
{
    grafo_elemento *aux;
    for (int i = 0; i < grafo->numero_vertices; i++)
    {
        if (grafo->vertices[i].chave == chave_1)
        {
            aux = grafo->vertices[i].prox;
            while (aux != NULL)
                aux = grafo->vertices[i].prox;
            aux = (grafo_elemento *)malloc(sizeof(grafo_elemento));
            aux->chave = chave_2;
            aux->distancia = distancia;
            break;
        }
    }
}

void liberar_grafo(grafos *grafo, int tamanho)
{
    grafo_elemento *aux_1, *aux_2;
    for (int i = 0; i < tamanho; i++)
    {
        aux_1 = grafo->vertices[i].prox;
        while (aux_1)
        {
            aux_2 = aux_1;
            aux_1 = aux_1->prox;
            free(aux_2);
        }
    }

    free(grafo->vertices);
    free(grafo);
}