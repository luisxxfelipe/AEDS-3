#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct arvore_binaria
{
    int valor;
    struct arvore_binaria *esquerda;
    struct arvore_binaria *direita;
} arvore;

arvore *inicializacao();
arvore *inserir_arvore(arvore **raiz, int valor);
arvore *remover_arvore(arvore *raiz, int valor);
arvore destruir_arvore(arvore *raiz);
arvore *buscar_arvore(arvore *raiz, int buscar);
void imprimir_arvore(arvore *raiz);
void pre_ordem(arvore *raiz);
void in_ordem(arvore *raiz);
void pos_ordem(arvore *raiz);
int contar_altura(arvore *raiz);
int contar_nos(arvore *raiz);

int main()
{
    arvore *s_arvore = inicializacao();
    int opcao, buscar, remover, valor;

    do
    {

        printf("\n\n[1]Adicionar valores na arvore.\n[2]Buscar os valores na arvore binaria.\n[3]Remover os valores na arvore binaria.\n[4]Quantidade de nos.\n[5]Destruir a arvore.\n[6]Verificar a altura da arvore.\n[7]Imprimir os elementos da arvore pela Pre-ordem.\n[8]Imprimir os elementos da arvore pela In-ordem.\n[9]Imprimir os elementos da arvore pela Pos-ordem.\n[10]Imprimir a arvore binaria.\n");
        printf("\n\nEscolha alguma opcao acima: ");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            printf("Digite o valor a ser adicionado: ");
            scanf("%d", &valor);
            inserir_arvore(&s_arvore, valor);
        }

        if (opcao == 2)
        {
            printf("\nDigite o valor que deseja buscar na arvore: ");
            scanf("%d", &buscar);
            buscar_arvore(s_arvore, buscar);
            system("pause");
            system("cls");
        }
        if (opcao == 3)
        {
            printf("\nDigite o valor que deseja remover na arvore: ");
            scanf("%d", &remover);
            remover_arvore(s_arvore, remover);
            system("pause");
            system("cls");
        }

        if (opcao == 4)
        {
            printf("\nA quantidade de nos na arvore sera %d\n", contar_nos(s_arvore));
            system("pause");
            system("cls");
        }

        if (opcao == 5)
        {
            destruir_arvore(s_arvore);
            printf("\nArvore destruida com sucesso!!\n");
            system("pause");
            system("cls");
        }

        if (opcao == 6)
        {
            printf("A altura da arvore sera %d\n", contar_altura(s_arvore));
        }

        if (opcao == 7)
        {
            pre_ordem(s_arvore);
        }

        if (opcao == 8)
        {
            in_ordem(s_arvore);
        }

        if (opcao == 9)
        {
            pos_ordem(s_arvore);
        }

        if (opcao == 10)
        {
            imprimir_arvore(s_arvore);
        }

    } while (opcao != 11);

    return 0;
}

arvore *inicializacao() // função de inicialização que iniciara a arvore como vazia
{
    return NULL;
}

arvore *inserir_arvore(arvore **raiz, int valor) // função para inserir os valores na arvore
{
    if ((*raiz) == NULL) // condição para acaso ainda nao tenha valor para a raiz
    {
        (*raiz) = (arvore *)malloc(sizeof(arvore)); // alocando o ponteiro da variavel raiz
        (*raiz)->valor = valor;                     // ponteiro recebera o valor inserido pelo usuario
        (*raiz)->esquerda = NULL;                   // inicializacao como o no esquerdo como vazio
        (*raiz)->direita = NULL;                    // inicializacao como o no direito como vazio
        printf("\nO valor foi inserido com sucesso na raiz!!\n");
    }

    else
    {
        if (valor < (*raiz)->valor) // acaso o valor inserido seja menor que o valor da raiz
        {
            (*raiz)->esquerda = inserir_arvore(&(*raiz)->esquerda, valor); // insere a esquerda da raiz
            printf("\nO valor foi inserido com sucesso na esquerda!!\n");
        }

        else if (valor > (*raiz)->valor) // acaso o valor inserido seja maior que o valor da raiz
        {
            (*raiz)->direita = inserir_arvore(&(*raiz)->direita, valor); // insere a direita da raiz
            printf("\nO valor foi inserido com sucesso na direita!!\n");
        }

        else
        {
            printf("\nInsercao nao realizada!! O valor %d, ja existe\n", valor);
        }
    }

    return (*raiz);
}

arvore *remover_arvore(arvore *raiz, int valor)
{
    if (raiz == NULL) // retorna como vazio o valor da raiz
    {
        return NULL;
    }

    else if (valor > raiz->valor) // acaso o valor inserido seja maior que o valor da raiz
    {
        raiz->direita = remover_arvore(raiz->direita, valor); // busca o valor ao lado direito e remove
    }

    else if (valor < raiz->valor) // acaso o valor inserido seja menor que o valor da raiz
    {
        raiz->esquerda = remover_arvore(raiz->esquerda, valor); // busva o valor ao lado esquerdo e remove
    }

    else
    {
        // nao possui nenhum filho
        if (raiz->esquerda == NULL && raiz->direita == NULL)
        {
            free(raiz);
            raiz = NULL; // remove somente a raiz
        }

        // nao possui filho na esquerda
        else if (raiz->esquerda == NULL)
        {
            arvore *aux = raiz;
            raiz = raiz->esquerda; // variavel aux passara a assumir o valor da raiz, depois o valor do ponteiro a esquerda e sera removido
            free(aux);
        }

        // nao possui filhos a direita
        else if (raiz->direita == NULL)
        {
            arvore *aux = raiz;
            raiz = raiz->direita; // variavel aux passara a assumir o valor da raiz, depois o valor do ponteiro a direita e sera removido
            free(aux);
        }

        else
        {
            arvore *aux = raiz->esquerda; // variavel aux passara a receber o valor da esquerda
            while (aux->direita != NULL)
            {
                aux = aux->direita; // aux passara a receber o valor da direita
            }

            // troca de informaçoes
            raiz->valor = aux->valor;
            aux->valor = valor;
            raiz->esquerda = remover_arvore(raiz, valor);
        }
        printf("\nO valor %d foi removido com sucesso\n\n", valor);
    }
}

arvore destruir_arvore(arvore *raiz) // limpa todos os vlaores inseridos
{
    if (raiz != NULL)
    {
        destruir_arvore(raiz->esquerda);
        destruir_arvore(raiz->direita);
        free(raiz);
    }
}

arvore *buscar_arvore(arvore *raiz, int buscar) // funcao que verifica se o valor inserido esteja do lado direito ou esquerdo da raiz
{
    if (raiz == NULL)
    {
        return NULL;
    }

    else
    {
        if (raiz->valor == buscar)
        {
            printf("\nO valor %d digitado foi encontrado na arvore!!!\n", buscar);
            return raiz;
        }

        else
        {
            if (buscar < raiz->valor)
            {
                return buscar_arvore(raiz->esquerda, buscar);
            }

            else
            {
                return buscar_arvore(raiz->direita, buscar);
            }
        }
    }
}

void imprimir_arvore(arvore *raiz) // imprime os valores do lado direito e esquerdo (incluindo a raiz)
{
    if (raiz != NULL)
    {
        imprimir_arvore(raiz->esquerda);
        imprimir_arvore(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

void pre_ordem(arvore *raiz) // imprime os valores na seguinte ordem: raiz, nó a esquerda, nó a direita
{
    if (raiz != NULL)
    {
        printf("%d ", raiz->valor);
        pre_ordem(raiz->esquerda);
        pre_ordem(raiz->direita);
    }
}

void in_ordem(arvore *raiz) // imprime os valores na seguinte ordem: nó a esquerda, raiz, nó a direita
{
    if (raiz != NULL)
    {
        pre_ordem(raiz->esquerda);
        printf("%d ", raiz->valor);
        pre_ordem(raiz->direita);
    }
}

void pos_ordem(arvore *raiz) // imprime os valores na seguinte ordem: nó a esquerda, nó a direita, raiz
{
    if (raiz != NULL)
    {
        pre_ordem(raiz->esquerda);
        pre_ordem(raiz->direita);
        printf("%d ", raiz->valor);
    }
}

int contar_altura(arvore *raiz)
{
    if (raiz == NULL)
    {
        return -1; // returna -1 acaso a arvore esteja vazia
    }

    else
    {
        int esq = contar_altura(raiz->esquerda);
        int dir = contar_altura(raiz->direita);

        if (esq > dir)
        {
            return esq + 1; // acaso o valor a esquerda seja maior que a direira, soma os valores da esquerda + 1 e retorna
        }

        else
        {
            return dir + 1; // acaso o valor direita seja maior que a direira, soma os valores da direita + 1 e retorna
        }
    }
}

int contar_nos(arvore *raiz)
{
    if (raiz == NULL)
    {
        return 0;
    }

    else
    {
        return 1 + contar_nos(raiz->esquerda) + contar_nos(raiz->direita); // conta os nos a esquerda da raiz + direita da raiz e retorna o resultado
    }
}