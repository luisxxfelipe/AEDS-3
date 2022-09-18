#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BRANCO 0
#define CINZA 1
#define PRETO 2

typedef struct Grafo
{
    int ponderado, n_vertices;                  // orientação e numero de vertices de um grafo
    int **arestas, **pesos;                     // vai guardar as conexoes, uma matriz alocada dinamicamente
    int *grau, grau_maximo;                     // para cada vertice quantas arestas ele possui
    int cor, tempo, tempo_inicial, tempo_final; // define a cor do no e o tempo inicial e final de busca
} s_grafo;

s_grafo *criar_grafo(int n_vertices, int grau_maximo, int ponderado);
s_grafo *arquivo_entrada();
int inserir_aresta(s_grafo *grafo, int origem, int destino, int digrafo, float peso);
void libera_grafo(s_grafo *grafo);
void imprimir_grafo(s_grafo *grafo);
void imprimir_arestas_adj(s_grafo *grafo, int origem);
void imprimir_vertices_adj(s_grafo *grafo, int v1);
void imprimir_vertices_incidentes(s_grafo *grafo, int vertice, int peso);
void imprimir_arestas_incidentes(s_grafo *grafo, int v1);
int imprimir_grau(s_grafo *grafo, int origem);
int sao_vertices_adj(s_grafo *grafo, int origem, int destino);
int trocar_vertices(s_grafo *grafo, int origem, int destino);
int substituir_peso(s_grafo *grafo, int origem_busq, int destino_busq, int novo_peso);
void algoritmo_dijkstra(s_grafo *grafo, int origem, int *anterior, float *distancia);
int procurar_menor_Distancia(float *distancia, int *visitado, int n_vertices);
void busca_em_largura(s_grafo *grafo, int origem, int *cor);
void profundidade(s_grafo *grafo, int origem, int *cor);
void busca_em_profundidade(s_grafo *grafo, int u, int *visitado, int contador);

int main()
{
    int opcao;
    int origem, destino, anterior[6], visitado[6];
    float distancia[6];
    int v1, v2, a1;
    s_grafo *grafo = arquivo_entrada();

    do
    {
        printf("\n\n[1]Imprimir todas as arestas adjacentes a uma aresta.\n[2]Imprimir todos os vertices adjacentes a um vertice.\n[3]Imprimir todas as arestas incidentes a um vertice.\n[4]Imprimir todos os vertices incidentes a uma aresta.\n[5]Imprimir o grau de um vertice.\n[6]Determinar se dois vertices sao adjacentes.\n[7]Substituir o peso de uma aresta.\n[8]Trocar dois vertices.\n[9]Algoritmo de dijkstra.\n[10]Busca em largura.\n[11]Busca em profundidade.\n\n");
        scanf("%d", &opcao);

        if (opcao == 1) // imprimir todas as arestas adjacentes a uma aresta
        {

            system("cls");
            imprimir_grafo(grafo);
            printf("\nInsira o vertice para verificar se existe arestas adjacentes: ");
            scanf("%d", &v1);
            imprimir_arestas_adj(grafo, v1);
            printf("\n");
            system("pause");
        }

        if (opcao == 2) // Imprimir todos os vertices adjacentes a um vertice
        {
            system("cls");
            printf("\nInsira o vertice para saber seus adjacentes: ");
            scanf("%d", &v1);
            imprimir_vertices_adj(grafo, v1);
            printf("\n");
            system("pause");
        }

        if (opcao == 3) // Imprimir todas as arestas incidentes a um vertice
        {
            system("cls");
            printf("\nInsira a aresta para saber suas arestas incidentes: ");
            scanf("%d", &a1);
            imprimir_arestas_incidentes(grafo, a1);
            printf("\n");
            system("pause");
        }

        if (opcao == 4) // Imprimr todos os vertices incidentes a uma aresta
        {
            system("cls");
            printf("\nInsira o vertice e a aresta para saber seus vertices incidentes: ");
            scanf("%d %d", &origem, &a1);
            imprimir_vertices_incidentes(grafo, origem, a1);
            printf("\n");
            system("pause");
        }

        if (opcao == 5) // Imprimir o grau de um vertice
        {
            system("cls");
            printf("\nDigite a origem do vertice: ");
            scanf("%d", &origem);
            imprimir_grau(grafo, origem);
            printf("\n");
            system("pause");
        }

        if (opcao == 6) // Determinar se dois vertices sao adjacentes
        {
            system("cls");
            printf("\nInsira os dois vertices para saber se sao adjacentes: ");
            scanf("%d %d", &v1, &v2);

            sao_vertices_adj(grafo, v1, v2);
            printf("\n");
            system("pause");
        }

        if (opcao == 7) // Substituir o peso de uma aresta
        {
            system("cls");
            int origem_busq, destino_busq, novo_peso;

            imprimir_grafo(grafo);

            printf("\nDigite o vetor e o vertice para a realizar a troca no grafo: ");
            scanf("%d %d", &origem_busq, &destino_busq);

            printf("\nInsira o novo peso da aresta: ");
            scanf("%d", &novo_peso);

            substituir_peso(grafo, origem_busq, destino_busq, novo_peso);
            printf("\n");
            system("pause");
        }

        if (opcao == 8) // Trocar dois vertices
        {
            system("cls");

            imprimir_grafo(grafo);

            printf("\nInsira o dois vertices que deseja realizar a troca entre si: ");
            scanf("%d %d", &origem, &destino);

            trocar_vertices(grafo, origem, destino);
            printf("\n");
            system("pause");
        }

        if (opcao == 9) // Algoritmo de Dijstra
        {

            system("cls");
            printf("\nIndique a origem do grafo que deseja percorre: ");
            scanf("%d", &origem);

            algoritmo_dijkstra(grafo, origem, anterior, distancia);

            for (int i = 0; i < grafo->n_vertices; i++)
            {
                printf("Vertice: %d: %d -> Distancia: %.2f.\n", i, anterior[i], distancia[i]);
            }
            printf("\n");

            system("pause");
        }

        if (opcao == 10) // Busca em largura
        {
            system("cls");
            int elemento, cor[6];

            printf("Insira a partir de qual elemento deseja buscar: ");
            scanf("%d", &elemento);
            busca_em_largura(grafo, elemento, cor);
            printf("\n");
            system("pause");
        }

        if (opcao == 11) // Busca em profundidade
        {
            system("cls");
            printf("\nIndique a origem do grafo que deseja percorre: ");
            scanf("%d", &origem);

            profundidade(grafo, origem, visitado);

            printf("\n");
            system("pause");
        }
    } while (opcao != 12);

    libera_grafo(grafo);

    return 0;
}

s_grafo *arquivo_entrada() // funcao para inserir no grafo as arestas, vertices e o grau a partir do arquivo
{
    int origem, destino, digrafo = 1, qtd_vertices = 0, qtd_arestas = 0, peso;
    FILE *arquivo = fopen("grafo.txt", "r");

    if (!arquivo)
    {
        printf("\nNao foi possivel abrir o arquivo.\n");
    }

    fscanf(arquivo, "%d %d", &qtd_vertices, &qtd_arestas);
    s_grafo *grafo = criar_grafo(qtd_arestas, qtd_vertices, 1);

    for (int i = 0; i < qtd_arestas; i++)
    {
        fscanf(arquivo, "%d %d %d", &origem, &destino, &peso);
        inserir_aresta(grafo, origem, destino, digrafo, peso);
    }

    return grafo;
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

        for (int i = 0; i < n_vertices; i++) // para cada vertice, to criando um grau maximo para ele (quantas arestas ele pode ter)
            grafo->arestas[i] = (int *)malloc(grau_maximo * sizeof(int));
        if (grafo->ponderado) // quando o grafo for ponderado, cria outra lista de adjancencia (matriz), onde cada posicao vai armazenar o peso
        {
            grafo->pesos = (int **)malloc(n_vertices * sizeof(int *)); // crio uma outra lista de adjacencia, onde cada posicao vai armazenar o peso
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

    // grafo->arestas e grafo->pesos funcionam como listas estaticas em que a quantidade de elementos em cada lista é dada por grafo->grau

    grafo->arestas[origem][grafo->grau[origem]] = destino; // vertice de destino sera armazenado na lista de arestas do vertice de origem, e na posicao seguinte ao ultimo valor da lista
    if (grafo->ponderado)                                  // se for ponderado ira adicionar o peso na matriz
        grafo->pesos[origem][grafo->grau[origem]] = peso;
    grafo->grau[origem]++; // soma mais um no grau do vertice de origem ou seja atualiza o numero de vertices a que o vertice de origem esta ligado

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
        printf("[%d]: ", i);
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

void imprimir_arestas_adj(s_grafo *grafo, int origem) // imprimir as arestas adjacentes
{
    for (int i = 0; i < grafo->n_vertices; i++)
    {
        for (int j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->arestas[i][origem] == grafo->arestas[i][j])
                printf("\nA arestas incidentes entre os vertices sao: V(%d):P(%d) ", grafo->arestas[i][j], grafo->pesos[i][j]);
        }
    }
}

void imprimir_vertices_adj(s_grafo *grafo, int v1) // imprimir os vertices adjacentes
{
    for (int i = 0; i < grafo->n_vertices; i++)
    {
        for (int j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->arestas[i][j] == grafo->arestas[v1][j])
                printf("\nO vertice (%d) e adjacente ao vertice (%d).\n", v1, grafo->arestas[i][j]);
        }
    }
}

void imprimir_vertices_incidentes(s_grafo *grafo, int vertice, int peso) // imprimir os vertices incidentes entre si
{
    for (int i = 0; i < grafo->n_vertices; i++)
    {
        for (int j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->arestas[vertice][j] == grafo->arestas[i][j])
                printf("\nO vertices incidentes a aresta de peso (%d), sao: (%d) - (%d).\n", peso, vertice, grafo->arestas[i][j]);
        }
    }
}

void imprimir_arestas_incidentes(s_grafo *grafo, int v1) // imprimir as arestas incidente entre si
{
    for (int i = 0; i < grafo->n_vertices; i++)
    {
        for (int j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->pesos[v1][j] == grafo->pesos[i][v1])
                printf("\nV(%d)::P(%d) e incidente ao: V(%d):P(%d).\n", v1, grafo->pesos[v1][j], grafo->arestas[i][j], grafo->pesos[i][j]);
        }
    }
}

int imprimir_grau(s_grafo *grafo, int origem) // imprimir o grau de cada vertice
{
    int cont = 0;
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices) // vetor nao pode ser menor que 0 e nao pode ser maior que o numero de vertices
        return 0;

    for (int i = 0; i < grafo->n_vertices; i++)
    {
        for (int j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->arestas[i][j] == origem)
            {
                cont++;
            }
        }
    }
    printf("\nO vertice (%d) tem grau (%d).\n", origem, cont);
}

int sao_vertices_adj(s_grafo *grafo, int origem, int destino) // verifica se o vertice inserido pelo usuario é adjacente ao segundo vertice inserido
{
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices) // vetor nao pode ser menor que 0 e nao pode ser maior que o numero de vertices
    {
        printf("\nO vertice de origem inserido nao existe no grafo!!!\n");
        return 0;
    }
    if (destino < 0 || destino >= grafo->n_vertices)
    {
        printf("\nO vertice de destino inserido nao existe no grafo!!!\n");
        return 0;
    }

    for (int i = 0; i < grafo->n_vertices; i++)
    {
        for (int j = 0; j < grafo->grau[i]; j++)
        {
            if (grafo->arestas[origem][j] == destino)
            {
                printf("\nO vertice %d e adjacente ao vertice %d.\n", origem, destino);
            }

            if (grafo->arestas[destino][j] == origem)
            {
                printf("\nO vertice %d e adjacente ao vertice %d.\n", origem, destino);
            }

            else
                return 0;
        }
    }
}

int substituir_peso(s_grafo *grafo, int origem, int destino, int novo_peso)
{
    if (grafo == NULL)
        return 0;

    if (origem < 0 || origem >= grafo->n_vertices) // vetor nao pode ser menor que 0 e nao pode ser maior que o numero de vertices
        return 0;

    if (destino < 0 || destino >= grafo->n_vertices) // inserindo no ultimo peso da origem
        return 0;

    for (int j = 0; j < grafo->grau[origem]; j++) // percorre pelo grau do vertice de origem
    {
        if (grafo->arestas[origem][j] == destino) // se for encontrado na aresta o vertice de origem
        {
            grafo->pesos[origem][j] = novo_peso; // incrementa o novo peso da aresta
        }
    }

    imprimir_grafo(grafo);
}

int trocar_vertices(s_grafo *grafo, int origem, int destino) // funcao para trocar os vertices inseridos pelo usuario
{
    int v1, v2, aux;
    if (grafo == NULL)
        return 0;
    if (origem < 0 || origem >= grafo->n_vertices) // vetor nao pode ser menor que 0 e nao pode ser maior que o numero de vertices
        return 0;

    if (origem < 0 || origem >= grafo->n_vertices)
        return 0;

    printf("\nDigite os numeros dos vetores que encontram o vertice 1 e o vertice 2: ");
    scanf("%d %d", &v1, &v2);

    for (int j = 0; j < grafo->grau[v1]; j++)
    {
        for (int k = 0; k < grafo->grau[v2]; k++)
        {
            if (grafo->arestas[v1][j] == origem)
            {
                if (grafo->arestas[v2][j] == destino)
                {
                    aux = grafo->arestas[v1][j];
                    grafo->arestas[v1][j] = grafo->arestas[v2][j];
                    grafo->arestas[v2][j] = aux;
                }
            }
        }
    }

    printf("\nO vertices foram trocados com sucesso!!\n\n");

    imprimir_grafo(grafo);
}

void busca_em_largura(s_grafo *grafo, int origem, int *visitado) // funcao que realizara a busca em largura do grafo
{
    int i, vert, n_vertices, cont = 1;
    int *fila, IF = 0, FF = 0;
    for (i = 0; i < grafo->n_vertices; i++)
    {
        visitado[i] = BRANCO; // defini todos os vertices como nao visitados
    }

    n_vertices = grafo->n_vertices;
    fila = (int *)malloc(n_vertices * sizeof(int)); // fila para guardar a ordem que os vertices foram visitados
    FF++;                                           // incrementa no final da fila
    fila[FF] = origem;                              // coloca o vertice inicial no final da fila
    visitado[origem] = cont;                             // e marca o vertice inicial como visitado
    while (IF != FF)                                // enquanto a fila nao estiver vazia
    {
        IF = (IF + 1) % n_vertices; // recupera o vertice q estao no inicio da fila
        vert = fila[IF];
        cont++;
        for (i = 0; i < grafo->grau[vert]; i++) // visita todos os vizinhos que ainda nao foram visitados
        {
            if (!visitado[grafo->arestas[vert][i]])
            {
                FF = (FF + 1) % n_vertices; // coloca ele na fila e marca ele como visitado
                fila[FF] = grafo->arestas[vert][i];
                visitado[grafo->arestas[vert][i]] = cont;
            }
        }
    }

    free(fila); // libera o vetor auxiliar

    for (i = 1; i < grafo->n_vertices; i++)
        printf("\nElemento atual: %d -> Elemento visitado: %d", i, visitado[i]);
}

void busca_em_profundidade(s_grafo *grafo, int origem, int *visitado, int contador) // funcao para realizar a busca em profundide do grafo
{
    visitado[origem] = contador; // marca o vertice inicial como visitado

    for (int i = 0; i < grafo->grau[origem]; i++) // percorro todas as arestas que partem do vertice
    {
        if (!visitado[grafo->arestas[origem][i]])
        {
            busca_em_profundidade(grafo, grafo->arestas[origem][i], visitado, contador + 1);
        }
    }
}

void profundidade(s_grafo *grafo, int origem, int *visitado) // funcao que acompanha a busca em profundidade
{
    printf("\nBusca iniciada...\n");
    int u, contador = 1;

    for (u = 0; u < grafo->n_vertices; u++)
    {
        visitado[u] = 0; // define incialmente como nenhum vertice visitado
    }

    busca_em_profundidade(grafo, origem, visitado, contador + 1); // visita recursivamente o grafo e retorna a sua origem
    
    for (int i = 0; i < grafo->n_vertices; i++)
    {
        printf("Vertice: [%d] - Visitado: [%d].\n", i, visitado[i]);
    }
}

int procurar_menor_Distancia(float *distancia, int *visitado, int n_vertices) // funcao para procurar a menor distancia
{
    int i, menor = -1, primeiro = 1;
    for (i = 0; i < n_vertices; i++)
    {
        if (distancia[i] >= 0 && visitado[i] == 0) // se o vertice ainda nao foi visitado, a distancia é -1
        {
            if (primeiro)
            {
                menor = i;
                primeiro = 0; // marca como primeiro
            }
            else // se nao é a primeira vez que encontra um vertice
            {
                if (distancia[menor] > distancia[i]) // distancia do menor > distancia do atual
                    menor = i;                       // faço a troca
            }
        }
    }

    return menor;
}

void algoritmo_dijkstra(s_grafo *grafo, int origem, int *anterior, float *distancia) // algoritmo de Dijkstra
{
    int i, cont, n_vertices, ind, *visitado, vert;
    cont = n_vertices = grafo->n_vertices; // contador para garantir que visitei todos os vertices
    visitado = (int *)malloc(n_vertices * sizeof(int));

    for (i = 0; i < n_vertices; i++) // inicializei as variaveis como n visitados
    {
        anterior[i] = -1;
        distancia[i] = -1;
        visitado[i] = 0;
    }

    distancia[origem] = 0;

    while (cont > 0) // enquanto tiver vertices para ser visitado
    {
        vert = procurar_menor_Distancia(distancia, visitado, n_vertices); // procura o vertice que tem menor distancia

        if (vert == -1) // nao alcançou o vertice
            break;

        visitado[vert] = 1; // marca como visitado
        cont--;             // visitei mais um vertice

        for (i = 0; i < grafo->grau[vert]; i++) // para cada vertice vizinho, ira fazer uma visita
        {
            ind = grafo->arestas[vert][i];
            if (distancia[ind] < 0) // ainda nao foi visitado
            {
                distancia[ind] = distancia[vert] + grafo->pesos[vert][i];
                anterior[ind] = vert; // para chegar no vertice ind, tem que chegar por vert
            }

            else
            {
                if (distancia[ind] > distancia[vert] + grafo->pesos[vert][i]) // se a distancia de ind for maior que vert + seu peso
                {
                    distancia[ind] = distancia[vert] + grafo->pesos[vert][i]; // significa que existe um caminho passando por vert que chega em ind com custo menor
                    anterior[ind] = vert;                                     // modifica quem era o antecessor
                }
            }
        }
    }

    free(visitado);
}