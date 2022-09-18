#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Sobre a escolha do n˙mero de chaves:
 * Foi escolhido um 2N Ìmpar para que o mÈtodo utilizado
 * por esta implementaÁ„o de ·rvore B n„o ferisse
 * as especificaÁıes. Ver descriÁ„o no inÌcio do cÛdigo.
 */
#define MAXIMO_CHAVES 55 /* 2n = 55; n = 27 */
#define TAMANHO_PAGINA 1024

/* Pequena macro para eliminar quebra de linha de WindowsÆ */
#define LIMPAR_QUEBRA(x)          \
    if (x[strlen(x) - 2] == 0x0D) \
    x[strlen(x) - 2] = '\0'
/* Macro para preencher com -1 um vetor. Usado para "anular" ponteiros
 * das p·ginas */
#define NULLIFICAR(x) memset(x, -1, (MAXIMO_CHAVES + 1) * sizeof(short))

/* ****************** DECLARA«’ES DE TIPOS ****************** */

/* Registro de elemento da ·rvore */
typedef struct CEP_indice
{
    char CEP[9];
    unsigned int byte_offset;
} CEP_indice;

/* RepresentaÁ„o de p·gina */
typedef struct Pagina
{
    unsigned short num_chaves;          /* n˙mero de chaves */
    CEP_indice entradas[MAXIMO_CHAVES]; /* chaves */
    short ponteiros[MAXIMO_CHAVES + 1]; /* ponteiros indicando a p·gina */
    char vazio[28];                     /* Usado para preencher a p·gina de 1024 bytes */
} Pagina;

/* RepresentaÁ„o da ·rvore */
typedef struct Arvore
{
    short paginas; /* P·ginas na ·rvore */
    short raiz;    /* RaÌz da ·rvore */
    int ponteiro;  /* MantÈm um indicador de posiÁ„o, evita seeks */
    FILE *fp;      /* Arquivo de Ìndices */
} Arvore;

/* ****************** VARI¡VEIS GLOBAIS ****************** */
#ifdef MEMORIA
/* Buffer de memÛria */
char buffer[2000000] = {};
#endif

/* ******************* PROT”TIPOS ****************** */
/* pagina_escrever
 * Escreve a p·gina dada por Pagina* no arquivo indicado por Arvore*
 * Nota: Se a p·gina for -1, a p·gina ser· adicionada no final
 * do arquivo/buffer, sendo a posiÁ„o da mesma indicada pelo retorno.
 *
 * entrada:
 * arv: ¡rvore.
 * pag: P·gina a ser escrita com seus dados.
 * pagina: N˙mero da p·gina a ser escrita.
 *
 * Retorno: O n˙mero da p·gina. */
int pagina_escrever(Arvore *arv, Pagina *pag, int pagina);

/* pagina_ler
 * LÍ a p·gina de Arvore* em Pagina*, indicado pelo n˙mero.
 *
 * entrada:
 * arv: ¡rvore.
 * pagina: n˙mero da p·gina a ser lida.
 *
 * saÌda:
 * pag: P·gina lida do arquivo no disco/buffer
 *
 * Retorno: O n˙mero da p·gina. */
int pagina_ler(Arvore *arv, Pagina *pag, int pagina);

/* pagina_split
 * FunÁ„o auxiliar utilizada para realizar a tÈcnica de "splitting"
 * nas p·ginas indicadas pelos n˙meros.
 *
 * entrada:
 * arv: ¡rvore
 * pag: P·gina que sofrer· 'splitting'.
 * pagina: n˙mero da p·gina que sofrer· 'splitting'.
 * pai: n˙mero da p·gina pai de p·gina, caso exista (-1 se n„o
 * existir).
 *
 * Retorno: O n˙mero da p·gina pai, sendo uma nova ou existente,
 * dependendo da vari·vel pai. */
int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai);

/* pagina_inserir
 * FunÁ„o respons·vel a inserir um elemento da p·gima em uma
 * posiÁ„o adequada atravÈs de ordenaÁ„o por inserÁ„o.
 *
 * entrada:
 * arv: ¡rvore
 * pag: P·gina em que ser· inserido elem
 * elem: Elemento a ser inserido
 *
 * Retorno: A posiÁ„o onde o novo elemento foi colocado. */
int pagina_inserir(Arvore *arv, Pagina *pag, CEP_indice elem);

/* arvore_inserir
 * FunÁ„o utilizada para inserir um elemento em uma ·rvore.
 *
 * entrada:
 * arv: ¡rvore.
 * elem: Elemento a ser inserido em ·rvore.
 *
 * Retorno: O n˙mero da p·gina onde foi inserido o elemento. */
int arvore_inserir(Arvore *arv, CEP_indice elem);

/* arvore_iniciar
 * FunÁ„o para iniciar uma ·rvore vazia.
 * NOTA: … preciso j· haver um arquivo (FILE*) designado a
 * essa ·rvore.
 *
 * entrada:
 * arv: ¡rvore.
 *
 * Retorno: N„o h· retorno. */
void arvore_iniciar(Arvore *arv);

/* ******************* IMPLEMENTA«√O ******************* */

int pagina_escrever(Arvore *arv, Pagina *pag, int pagina)
{
#ifdef MEMORIA
    if (pagina < 0)
    {
        arv->ponteiro += TAMANHO_PAGINA;
        memcpy(&buffer[arv->ponteiro], pag, TAMANHO_PAGINA);
        return arv->ponteiro / TAMANHO_PAGINA;
    }
    else
    {
        memcpy(&buffer[pagina * TAMANHO_PAGINA], pag, TAMANHO_PAGINA);
        return pagina;
    }
#else
    if (pagina < 0)
    {
        arv->ponteiro += TAMANHO_PAGINA;
        fseek(arv->fp, 0, SEEK_END);
        fwrite(pag, TAMANHO_PAGINA, 1, arv->fp);
        return arv->ponteiro / TAMANHO_PAGINA;
    }
    else
    {
        fseek(arv->fp, pagina * TAMANHO_PAGINA, SEEK_SET);
        fwrite(pag, TAMANHO_PAGINA, 1, arv->fp);
        return pagina;
    }
#endif
    return 0;
}

int pagina_ler(Arvore *arv, Pagina *pag, int pagina)
{
#ifdef MEMORIA
    memcpy(pag, &buffer[pagina * TAMANHO_PAGINA], TAMANHO_PAGINA);
    return pagina;
#else
    fseek(arv->fp, pagina * TAMANHO_PAGINA, SEEK_SET);
    fread(pag, TAMANHO_PAGINA, 1, arv->fp);
    return pagina;
#endif
}

int pagina_split(Arvore *arv, Pagina *pag, int pagina, int pai)
{
    Pagina pag_nova;
    Pagina pag_pai;
    int local = 0; /* local onde ser· inserido o elemento promovido */
    int nova = 0;  /* n˙mero da nova p·gina */
    int medio = 0; /* posiÁ„o mediana */
    int i = 0;

    /* Iniciar as p·ginas novas
     * Se o pai j· existir, lÍ-lo, caso contr·rio, cria-se um novo nÛ */
    if (pai >= 0)
    {
        pagina_ler(arv, &pag_pai, pai);
    }
    else
    {
        pag_pai.num_chaves = 0;
        memset(pag_pai.entradas, 0, MAXIMO_CHAVES * sizeof(CEP_indice));
        NULLIFICAR(pag_pai.ponteiros);
    }

    /* Cria a nova p·gina */
    pag_nova.num_chaves = 0;
    memset(pag_nova.entradas, 0, MAXIMO_CHAVES * sizeof(CEP_indice));
    NULLIFICAR(pag_nova.ponteiros);

    /* MÈdio È: elementos + mÈdio + elementos: sÛ que o elemento
     * m·ximo È m·ximo - 1, pois È baseado no zero. */
    medio = (MAXIMO_CHAVES - 1) / 2;

    /* Insere os elementos da p·gina a sofrer 'splitting' na p·gina
     * nova e remove os mesmos da p·gina original. */
    for (i = medio + 1; i < MAXIMO_CHAVES; i++)
    {
        /* i-(medio+1) faz com que o laÁo comece em 0 para
         * a p·gina nova, que vai receber os novos elementos. */
        pag_nova.entradas[i - (medio + 1)] = pag->entradas[i];
        pag->entradas[i].CEP[0] = '\0'; /* Apaga o elemento,
                                           anulando a string. */

        /* Move agora os ponteiros. */
        pag_nova.ponteiros[i - medio] = pag->ponteiros[i + 1];
        pag->ponteiros[i + 1] = -1;
    }
    pag_nova.num_chaves = medio;
    pag->num_chaves = medio;

    /* Move agora o ponteiro final de pag para nova */
    pag_nova.ponteiros[0] = pag->ponteiros[medio + 1];
    pag->ponteiros[medio + 1] = -1;

    nova = pagina_escrever(arv, &pag_nova, -1);

    /* InÌcio do "promoting". */
    /* Agora insere o elemento mediano na p·gina pai. */
    local = pagina_inserir(arv, &pag_pai, pag->entradas[medio]);
    pag->entradas[medio].CEP[0] = '\0';

    /* Coloca os ponteiros no pai. */
    pag_pai.ponteiros[local] = pagina;
    pag_pai.ponteiros[local + 1] = nova;

    /* Atualiza a ·rvore */
    if (pai == -1)
        arv->paginas += 2;
    else
        arv->paginas++;
    pai = pagina_escrever(arv, &pag_pai, pai);

    pagina_escrever(arv, pag, pagina);

#ifdef DEBUG
    printf("Split: %d para %d e %d\n", pagina, nova, pai);
#endif

    if (pagina == arv->raiz)
    {
        arv->raiz = pai;
#ifdef DEBUG
        printf("Atualizando raiz: %d\n", arv->raiz);
#endif
    }

    return pai;
}

int pagina_inserir(Arvore *arv, Pagina *pag, CEP_indice elem)
{
    int i = 0;
    int j = 0;
    int finalizado = 0;
    int posicao_ideal = -1;

    pag->num_chaves++;

    /* Primeiramente, procurar por um lugar vazio no vetor. */
    for (i = 0; i < pag->num_chaves && !finalizado; i++)
    {
        if (strcmp(pag->entradas[i].CEP, elem.CEP) > 0)
        {
            posicao_ideal = i;
            finalizado = 1;
        }
    }
    /* Se n„o encontrou posiÁ„o ideal e h· mais de uma chave
     * no vetor, ent„o a posiÁ„o ideal È no final do vetor. */
    if (posicao_ideal < 0)
    {
        posicao_ideal = pag->num_chaves - 1;
    }

    j = posicao_ideal;

    /* Procura um espaÁo vazio. */
    while (strlen(pag->entradas[j].CEP) != 0)
        j++;

    while (posicao_ideal != j)
    {
        pag->ponteiros[j + 1] = pag->ponteiros[j]; /* move o ponteiro adiantado */
        pag->ponteiros[j] = -1;

        pag->entradas[j] = pag->entradas[j - 1];
        j--;
    }

    /* Move os ponteiros e finalmente aloca o elemento em sua posiÁ„o. */
    pag->ponteiros[posicao_ideal + 1] = pag->ponteiros[posicao_ideal];
    pag->ponteiros[posicao_ideal] = -1;
    pag->entradas[posicao_ideal] = elem;

    return posicao_ideal;
}

int arvore_inserir(Arvore *arv, CEP_indice elem)
{
    int i;
    int pagina_pai = -1;
    int pagina_atual;
    int fim_pagina = 0;
    Pagina pag;

    pagina_ler(arv, &pag, arv->raiz);
    pagina_atual = arv->raiz;

    if (pagina_atual == arv->raiz && pag.num_chaves == MAXIMO_CHAVES)
    {
        pagina_atual = pagina_split(arv, &pag, pagina_atual, -1);
        pagina_ler(arv, &pag, arv->raiz);
        pagina_pai = -1;
    }

    while (1)
    {
        for (i = 0; i <= pag.num_chaves && !fim_pagina; i++)
        {
            /* Checa-se se È o ˙ltimo ponteiro da p·gina ou se È um local adequado
             * a se buscar um ponteiro por p·gina */
            if (i == pag.num_chaves || strcmp(pag.entradas[i].CEP, elem.CEP) > 0)
            {
                if (pag.ponteiros[i] != -1)
                {
                    pagina_pai = pagina_atual;
                    pagina_atual = pag.ponteiros[i];
                    pagina_ler(arv, &pag, pagina_atual);

                    if (pag.num_chaves == MAXIMO_CHAVES)
                    {
                        pagina_atual = pagina_split(arv, &pag, pagina_atual, pagina_pai);
                        pagina_ler(arv, &pag, pagina_atual);
                        pagina_pai = -1;
                    }
                    fim_pagina = 1;
                }
                else
                {
                    /* Inserir na p·gina, caso a mesma n„o esteja vazia. */
#ifdef DEBUG
                    printf("Inserindo %s na p·gina %d\n", elem.CEP, pagina_atual);
#endif
                    pagina_inserir(arv, &pag, elem);
                    pagina_escrever(arv, &pag, pagina_atual);
                    return pagina_atual; /* Ponto de saÌda da funÁ„o, apÛs a inserÁ„o. */
                }
            }
        }
        fim_pagina = 0;
    }
    return 0;
}

long arvore_busca(Arvore *arv, const char *CEP)
{
    Pagina pag;
    int i = 0;
    int comparacao = 0;
    int finalizado_arvore = 0;
    int finalizado_pagina = 0;

    pagina_ler(arv, &pag, arv->raiz);
    while (!finalizado_arvore)
    {
        /* Itera entre os ponteiros da ·rvore. */
        for (i = 0; i < pag.num_chaves && !finalizado_pagina; i++)
        {
            comparacao = strcmp(pag.entradas[i].CEP, CEP);
            /* Se o CEP a ser buscado È menor que o elemento atual, 'entra
             * no ponteiro'. */
            if (comparacao > 0)
            {
                if (pag.ponteiros[i] >= 0)
                {
                    pagina_ler(arv, &pag, pag.ponteiros[i]);
                    finalizado_pagina = 1;
                }
                else
                    return -1; /* Se n„o existe ponteiro, acabou a busca. */
            }
            else
            {
                if (comparacao == 0)
                {
                    return (long)pag.entradas[i].byte_offset;
                }
            }
        }
        /* Se saiu do laÁo, ou È porque achamos uma p·gina e vamos
         * comeÁar a leitura nela (finalizado_pagina = 1) ou
         * È porque n„o encontramos candidatos para tal. Fazer uma
         * ˙ltima tentativa no ponteiro p+1. Se ele n„o existir,
         * terminamos uma busca que n„o achou. */
        if (finalizado_pagina)
        {
            finalizado_pagina = 0;
        }
        else
        {
            if (pag.ponteiros[i] >= 0)
            {
                finalizado_pagina = 0;
                pagina_ler(arv, &pag, pag.ponteiros[i]);
            }
            else
                finalizado_arvore = 1; /* Se n„o encontramos nada... */
        }
    }
    return -1;
}

void arvore_iniciar(Arvore *arv)
{
    Pagina pag;

    arv->paginas = 1;
    arv->raiz = 0;
    arv->ponteiro = -1024; /* ComeÁa com -1024 para facilitar a funÁ„o de escrita. */

    pag.num_chaves = 0;
    NULLIFICAR(pag.ponteiros);
    memset(pag.entradas, 0, MAXIMO_CHAVES * sizeof(CEP_indice));

    pagina_escrever(arv, &pag, -1);
}

int main()
{
    long offset = 0;
    char buff[1000] = {};
    FILE *ceps = NULL;
    FILE *consultas = NULL;
    CEP_indice temp;
    Arvore arvore;
#ifdef DEBUG
    Pagina pag;
    int j;
    int i;
#endif

    arvore.fp = fopen("indices.dat", "wb+");
    arvore_iniciar(&arvore);

    ceps = fopen("cep.txt", "rb");

    while (!feof(ceps))
    {
        offset = ftell(ceps);

        fgets(buff, 1000, ceps);

        memcpy(temp.CEP, buff, 8);
        temp.CEP[8] = '\0';
        temp.byte_offset = offset; /* Pega o byte offset, adquirido anteriormente. */

        arvore_inserir(&arvore, temp);
    }

#ifdef DEBUG
    printf("Raiz: %d\n", arvore.raiz);
    printf("Imprimindo %d paginas\n", arvore.paginas);
    for (i = 0; i < arvore.paginas; i++)
    {
        printf("Imprimindo p·gina %d\n", i);
        pagina_ler(&arvore, &pag, i);
        for (j = 0; j < pag.num_chaves; j++)
        {
            printf("ponteiro[%d]: = %d\nelemento[%d] = %s\n", j, pag.ponteiros[j], j, pag.entradas[j].CEP);
        }
        printf("ponteiro[%d]: = %d\n", j, pag.ponteiros[j]);
        printf("--------------------\n");
    }
#endif

    consultas = fopen("consultas.txt", "r");
    while (!feof(consultas))
    {
        fgets(buff, 1000, consultas);
        LIMPAR_QUEBRA(buff);
        offset = arvore_busca(&arvore, buff);
        if (offset < 0)
        {
            printf("%s NAO ENCONTRADO\n", buff);
        }
        else
        {
            fseek(ceps, offset, SEEK_SET);
            fgets(buff, 1000, ceps);
            LIMPAR_QUEBRA(buff);
            printf("%s\n", buff);
        }
    }
    fclose(arvore.fp);
    fclose(consultas);
    fclose(ceps);
    return 0;
}