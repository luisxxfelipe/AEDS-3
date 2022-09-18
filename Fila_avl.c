#include <stdio.h>
#include <stdlib.h>

typedef struct s_fila
{
    int dado;
    struct s_fila *valor;
} s_fila;

typedef struct s_aux
{
    struct s_fila *inicio;
    struct s_fila *fim;
} s_aux;

s_aux *criar_fila();
int fila_vazia(s_aux *f);
int inserir_fila(int dados);
int remover_fila(s_aux *f);
void exibir_fila(s_aux *f);
int mostrar_primeiro(s_aux *f);
int destruir_fila(s_aux *f);

int main()
{
    int opcao, valores;

    s_aux *f;
    
    criar_fila();

    do
    {
        printf("\n\n[1]Adicionar valores para a fila.\n[2]Remover valor da fila.\n[3]Mostrar os elementos da fila.\n[4]Destruir fila.\n[5]Mostrar o primeiro elemento da fila.\n[6]Sair do programa.\n");
        printf("\n\nEscolha alguma opcao acima: ");
        scanf("%d", &opcao);

        if (opcao == 1)
        {
            printf("\n\nDigite os valores para a fila: \n");
            scanf("%d", &valores);
            inserir_fila(valores);

            system("pause");
            system("cls");
        }

        else if (opcao == 2)
        {
            remover_fila(f);

            system("pause");
            system("cls");
        }

        else if (opcao == 3)
        {
            printf("\nos valores que estao na fila sao: \n");
            exibir_fila(f);

            system("pause");
            system("cls");
        }

        else if (opcao == 4)
        {
            destruir_fila(f);

            system("pause");
            system("cls");
        }

        else if (opcao == 5)
        {
            mostrar_primeiro(f);

            system("pause");
            system("cls");
        }

    } while (!(opcao == 6));

    return 0;
}

s_aux *criar_fila()
{
    s_aux *f = (s_aux *)malloc(sizeof(s_aux));

    if (!f)
    {
        exit(1);
    }

    else
    {
        f->inicio = NULL;
        f->fim = NULL;
    }

    return f;
}

int inserir_fila(int dados)
{
    s_fila *aux;
    aux = (s_fila *)malloc(sizeof(s_fila));

    if (!aux)
    exit(1);

      aux->dado = dados;
        aux->valor = NULL;

    printf("\n\nValor foi inserido com sucesso!!\n");

    return 1;
}

int remover_fila(s_aux *f)
{
    s_fila *aux;

    if (f->inicio == NULL)
    {
        printf("\n\nA fila esta vazia!!\n\n");

        return 0;
    }

    else
    {
        aux = f->inicio;
        f->inicio = f->inicio->valor;

        if (f->inicio == NULL)

            f->fim = NULL;

        free(aux);

        printf("\n\nElemento removido com sucesso\n");
    }
    return 1;
}

void exibir_fila(s_aux *f)
{
    s_fila *aux = f->fim;

    if (f->inicio == NULL)
    {
        printf("\n\nA fila esta vazia!!\n\n");
    }

    else
    {
        while (aux != NULL)
        {
            printf(" %d |", aux->dado);
            aux = aux->valor;
        }
    }
}

int mostrar_primeiro(s_aux *f)
{
    int valor;
    if (f->inicio == NULL)
    {
        printf("\n\nA fila esta vazia!!\n\n");
        return 0;
    }

    else
    {
        valor = f->inicio->dado;
        printf("O primeiro elemento sera %d", valor);
        return 1;
    }
}

int destruir_fila(s_aux *f)
{
    s_fila *aux;
    if (f->inicio == NULL)
    {
        printf("\n\nA fila esta vazia!!\n\n");
        return 0;
    }
    else
    {
        while (f->inicio != NULL)
        {
            aux = f->inicio;
            f->inicio = f->inicio->valor;
            free(aux);
        }
        printf("\n\nA fila esta vazia!!\n\n");
        return 1;
    }
}