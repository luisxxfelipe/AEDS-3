#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int main()
{
    int result;
    result = 0;
    // Menu de escolha
    printf("Matriz Adjacente de Grafos ou Digrafos!");
    printf("\n\n Escolha: (1) Grafos (2) Digrafos :");

    scanf("%d", &result);
    system("cls");
    // Condição de GRAFO ou DIGRAFO
    if (result == 1)
    {
        printf("\nGRAFO SELECIONADO\n ");
    }
    else
    {
        if (result == 2)
        {
            printf("\nDIGRAFO SELECIONADO\n ");
        }
        // se nenhuma opção for valida, gera um erro e volta o programa
        else
        {
            printf("\nOPCAO INVALIDA!!\n");
        }
    }
    // definição da variavel Vertice
    int i, j, x, arestas, vertices, origem, destino;
    printf("\nQuantidade de Vertices: ");
    scanf("%d", &vertices);

    vertices = vertices + 1;
    int matriz[vertices][vertices]; // inicia matriz

    // inicia todos os pontos da matriz como "0"
    for (i = 0; i < vertices; i++)
        for (j = 0; j < vertices; j++)
            matriz[i][j] = 0;

    // implementa titulo de linha e coluna na matriz
    for (i = 0; i < vertices; i++)
    {
        matriz[i][0] = i;
    }
    for (j = 0; j < vertices; j++)
    {
        matriz[0][j] = j;
    }
    // Colocar pontos na matriz
    printf("\nQuantidade de Arestas: ");
    scanf("%d", &arestas);

    for (x = 0; x < arestas; x++)
    {
        printf("\nVertice de Origem %d: ", x + 1);
        scanf("%d", &origem);
        if (origem > vertices - 1)
        { // verifica se vertice existe na matriz
            printf("\nPosicao de Vertice de Origem nao existente");
            printf("\n\nInsira novamente\n\n");
        }

        printf("\nVertice de Destino %d: ", x + 1);
        scanf("%d", &destino);
        if (destino > vertices - 1)
        { // verifica se vertice existe na matriz
            printf("\nPosicao de Vertice de Destino nao existente");
            printf("\n\nInsira novamente\n\n");
        }

        switch (result)
        {       // define entre GRAFO e DIGRAFO usando a varialvel "result" definida no começo do programa
        case 1: // GRAFO
            matriz[origem][destino] = 1;
            matriz[destino][origem] = 1;
            break;
        case 2: // DIGRAFO
            matriz[origem][destino] = 1;
            break;
        }
        // print da matriz
        for (i = 0; i < vertices; i++)
        {
            printf("\n");
            for (j = 0; j < vertices; j++)
                printf("%d ", matriz[i][j]);
        }
        printf("\n\n");
    }
    printf("      Tecle !\n\n");
    getch();
    return 0;
}

