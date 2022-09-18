#include <stdio.h>
#include <stdlib.h>

typedef struct s_aluno
{
    int matricula;
} aluno;

typedef aluno *t_hash; // t_hash é um vetor que será alocado dinamicamente

void inicializacao(t_hash *tabela_hash, int vetor) // função para inicializar e definir o tamanho do vetor
{
    int i;
    for (i = 0; i < vetor; i++)
    {
        tabela_hash[i] = NULL;
    }
}

aluno *alocar_memoria(int dados_matricula) // alocação de memoria no vetor
{
    aluno *novo = (aluno *)malloc(sizeof(aluno));
    novo->matricula = dados_matricula;
    return novo;
}

int calculo_hash(int dados_matricula, int vetor) // calculo para determinação o local que o elemento vai ser colocado n % vetor
{
    return dados_matricula % vetor;
}

void liberar_tabela(t_hash *tabela_hash, int vetor) // liberar todos os vetores inseridos (free)
{
    int i;
    for (i = 0; i < vetor; i++)
    {
        if (tabela_hash[i])
        {
            free(tabela_hash[i]);
        }
    }
}

int buscar_tabela(t_hash *tabela_hash, int vetor, int dados_matricula) // função para buscar os elementos na tabela hash
{
    int pos = calculo_hash(dados_matricula, vetor);
    if (tabela_hash[pos] != NULL)
    {
        aluno *resp = tabela_hash[pos];
        if (resp->matricula == dados_matricula)
        {
            printf("\nA matricula na posicao [%d], foi encontrada.\n", dados_matricula);
            return pos;
        }
    }
    return -1;
}

void inserir_tabela(t_hash *tabela_hash, int vetor, int dados_matricula) // função para realizar a inserção na tabela e tratando as colisoes
{

    int indice = calculo_hash(dados_matricula, vetor);
    if (tabela_hash[indice] == NULL)
    {
        tabela_hash[indice] = alocar_memoria(dados_matricula);
        printf("\nElemento inserido com sucesso!!\n");
    }

    else
    {
        for (int i = 0; i < vetor; i++)
        {
            if (tabela_hash[indice] != NULL) // enquanto o vetor nao esteja vazio, percorrera + 1 no indice para analisar se existe espaço e assim inserir
            {
                tabela_hash[indice + 1] = alocar_memoria(dados_matricula);
            }
        }
    }
}

void imprimir_tabela(t_hash *tabela_hash, int vetor) // função para imprimir a tabela hash
{
    int h;
    printf("\n------------------------TABELA---------------------------\n");
    for (int i = 0; i < vetor; i++)
    {
        h = i;
        printf("%d:\n", h);

        if (tabela_hash[h] != NULL)
        {
            aluno *p = tabela_hash[h];
            printf("Elemento: %d\n", p->matricula);
        }
    }
    printf("\n----------------------------------------------------------\n");
}

int main()
{
    int n, vetor;
    printf("Digite o tamanho do vetor: \n");
    scanf("%d", &vetor);
    printf("Digite a quantidade de matriculas que deseja adicionar: \n");
    scanf("%d", &n);

    if ((n <= 1) || (vetor <= 1))
        return 0;

    t_hash tabela_hash[vetor];
    inicializacao(tabela_hash, vetor);

    int i, dados_matricula;
    for (i = 0; i < n; i++)
    {
        printf("Digite a matricula: \n");
        scanf("%d", &dados_matricula);
        inserir_tabela(tabela_hash, vetor, dados_matricula);
    }
    imprimir_tabela(tabela_hash, vetor);

    char resp;

    do
    {
        printf("Digite a matricula a ser procurada: \n");
        scanf("%d", &dados_matricula);
        int ind = buscar_tabela(tabela_hash, vetor, dados_matricula);
        if (ind == -1)
            printf("Elemento nao encontrado!\n");
        else
        {
            aluno *p = tabela_hash[ind];
            printf("%d\t\n", p->matricula);
        }
        printf("Digita 1 se deseja continuar com a busca: \n");
        scanf(" %c", &resp);
    } while ((resp != 'N') && (resp != 'n'));
    imprimir_tabela(tabela_hash, vetor);

    liberar_tabela(tabela_hash, vetor);

    return 0;
}