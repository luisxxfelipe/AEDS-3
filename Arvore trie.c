#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

typedef enum verificar_ocupado
{
    ocupado,
    desocupado
} verificar;

typedef struct arvore_trie
{
    int valor;
    struct s_arvore *filhos[256];
    verificar estado;
} s_arvore;

s_arvore *criar_no();
void inserir_arvore(s_arvore **raiz, unsigned char *chave, int valor);
void remocao_arvore(s_arvore **raiz, unsigned char *chave);
s_arvore *buscar_arvore(s_arvore *raiz, unsigned char *chave);
s_arvore destruir_arvore(s_arvore *raiz);

int main()
{
    s_arvore *raiz = NULL;
    int opcao, valor;

    do
    {

        printf("\n[1]Adicionar valores na arvore trie.\n[2]Buscar os valores na arvore trie.\n[3]Remover os valores na arvore trie.\n[4]Destruir a arvore.\n");
        printf("\n\nEscolha alguma opcao acima: ");
        scanf("%d", &opcao);
        unsigned char buscar[256], remover[256], palavra[256];

        if (opcao == 1)
        {
            printf("\nDigite a palavra a ser inserida: \n");
            scanf("%s", palavra);

            printf("\nDigite o valor que representara a palavra: \n");
            scanf("%d", &valor);

            inserir_arvore(&raiz, palavra, valor);
        }

        if (opcao == 2)
        {
            printf("\nDigite a palavra que deseja procurar: \n");
            scanf("%s", buscar);

            buscar_arvore(raiz, buscar);
        }

        if (opcao == 3)
        {
            printf("\nDigite a palavra que deseja remover: \n");
            scanf("%s", remover);
            remocao_arvore(&raiz, remover);
            printf("\nA palavra %s foi removida com sucesso!!\n", remover);
        }

        if (opcao == 4)
        {
            destruir_arvore(raiz);
            printf("\nArvore destruida com sucesso!!\n");
        }
    } while (opcao != 5);
    return 0;
}

s_arvore *criar_no()
{ // inicilizando os valores de estado, valor e filhos a 0
    int i;
    s_arvore *trie;
    trie = malloc(sizeof(s_arvore));
    trie->estado = desocupado;
    trie->valor = 0;

    for (i = 0; i < 256; i++)
    {
        trie->filhos[i] = NULL;
    }

    return trie;
}

void inserir_arvore_R(s_arvore **raiz, unsigned char *chave, int valor, int tamanho_chave, int buscas_realizadas)
{
    if (*raiz == NULL)
    {
        *raiz = criar_no(); // inserindo um novo nó no caminho do prefixo, caso nao tenha a letra em seguida
    }

    if (buscas_realizadas == tamanho_chave)
    {
        (*raiz)->valor = valor; // o ponteiro valor passa a assumir o novo valor inserido e, por o fim, o estado fica ocupado
        (*raiz)->estado = ocupado;
        printf("\nA palavra foi inserida com sucesso!!!\n");
        return;
    }

    inserir_arvore_R(&(*raiz)->filhos[chave[buscas_realizadas]], chave, valor, tamanho_chave, buscas_realizadas + 1);
}

void inserir_arvore(s_arvore **raiz, unsigned char *chave, int valor)
{
    inserir_arvore_R(raiz, chave, valor, strlen(chave), 0);
}

void remocao_arvore_R(s_arvore **raiz, unsigned char *chave, int tamanho_chave, int buscas_realizadas)
{
    int i;
    if ((*raiz) == NULL) // palavra nao esta na s_arvore e retorna NULL
    {
        return;
    }

    if (buscas_realizadas == tamanho_chave) // correspondeu as letras da palavra escolhida
    {
        (*raiz)->estado = desocupado; // o valor que estava associado aquele nó vai passar a deixar de existir e vai ser ocupado
    }
    else
    {

        remocao_arvore_R(&(*raiz)->filhos[chave[buscas_realizadas]], chave, tamanho_chave, buscas_realizadas + 1);
    }

    if ((*raiz)->estado == ocupado) // representa o valor de uma chave que nao foi removida, portanto nao remove
    {
        return;
    }
    for (i = 0; i < 256; i++)
    {
        if ((*raiz)->filhos[i] != NULL) // se filho for diferente de NULL o nó nao vai ser removido
        return;
    }

    free(*raiz);  // limpa o nó e atribui ele para NULL,
    *raiz = NULL; //"desconecta" o no do restante da trie
}

void remocao_arvore(s_arvore **raiz, unsigned char *chave)
{
    remocao_arvore_R(raiz, chave, strlen(chave), 0);
}

s_arvore *buscar_arvore_R(s_arvore *raiz, unsigned char *chave, int tamanho_chave, int buscas_realizadas)
{
    if (raiz == NULL) // nao existe o no procurado pelo nome inserido para busca
    {
        printf("\nA arvore se encontra vazia ou nao possui o nome encontrado.\n");
        return NULL;
    }

    if (buscas_realizadas == tamanho_chave) // encontrei o nome procurado pelo usuario
    {
        printf("\nA palavra buscada foi encontrada.\n");
        return raiz; // retorna o valor buscado
    }

    return buscar_arvore_R(raiz->filhos[chave[buscas_realizadas]], chave, tamanho_chave, buscas_realizadas + 1);
}

s_arvore *buscar_arvore(s_arvore *raiz, unsigned char *chave)
{
    buscar_arvore_R(raiz, chave, strlen(chave), 0);
}

s_arvore destruir_arvore(s_arvore *raiz)
{
    if (raiz != NULL)
    {
        destruir_arvore(raiz);
        free(raiz);
    }
}