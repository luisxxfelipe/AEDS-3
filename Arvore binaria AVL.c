#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

typedef struct s_arvore
{
    char palavra[20];          // elemento a inserir na arvore
    struct s_arvore *Esquerda; // Ligação com outro nó pela esquerda
    struct s_arvore *Direita;  // Ligação com outro nó pela direita
    int altura;                // altura do Nó
} s_arvore;

int pont_NULL(s_arvore *raiz);
int altura(s_arvore *raiz);
int Maior(int a, int b);
s_arvore *RotDir(s_arvore *raiz);
s_arvore *RotEsq(s_arvore *raiz);
int Balanceamento(s_arvore *raiz);
s_arvore *inserir_arvore(s_arvore *raiz, char palavra[20]);

int main()
{

}

int altura(s_arvore *raiz) // retorna a altura da arvore
{
    if (raiz == NULL)    // Se raiz for nulo
        return -1;       // retorna 0
    return raiz->altura; // Senão usa recursividade
}

int Maior(int a, int b) // Função que retorna o maior valor entre dois inteiros (Utilizado pelas rotações
{
    return (a > b) ? a : b;
}

s_arvore *RotDir(s_arvore *raiz) // Função para a rotação da sub-árvore (à direita)
{
    s_arvore *AuxiliarEsquerda = raiz->Esquerda;
    s_arvore *AuxiliarDireita = AuxiliarEsquerda->Direita;

    // Realizando rotação
    AuxiliarEsquerda->Direita = raiz;
    raiz->Esquerda = AuxiliarDireita;

    // Atualizando alturas
    raiz->altura = Maior(altura(raiz->Esquerda), altura(raiz->Direita)) + 1;
    AuxiliarEsquerda->altura = Maior(altura(AuxiliarEsquerda->Esquerda), altura(AuxiliarEsquerda->Direita)) + 1;

    // Retornando a nova raiz (raiz alterada)
    return AuxiliarEsquerda;
}

s_arvore *RotEsq(s_arvore *raiz) // Função para a rotação da sub-árvore (à esquerda)
{
    s_arvore *AuxiliarDireita = raiz->Direita;
    s_arvore *AuxiliarEsquerda = AuxiliarDireita->Esquerda;

    // Realizando rotação
    AuxiliarDireita->Esquerda = raiz;
    raiz->Direita = AuxiliarEsquerda;

    // Atualizando alturas
    raiz->altura = Maior(altura(raiz->Esquerda), altura(raiz->Direita)) + 1;
    AuxiliarDireita->altura = Maior(altura(AuxiliarDireita->Esquerda), altura(AuxiliarDireita->Direita)) + 1;

    // Retornando a nova raiz (raiz alterada)
    return AuxiliarDireita;
}

int pont_NULL(s_arvore *raiz) // Função para verificar se o ponteiro raiz do tipo s_arvore (registro) é nulo
{
    return (raiz == (s_arvore *)NULL); // Sempre retorna nulo se for
}

int Balanceamento(s_arvore *raiz) // Função que retorna o fator de balanceamento do nó
{
    if (raiz == NULL)
        return 0;
    return altura(raiz->Esquerda) - altura(raiz->Direita);
}

s_arvore *inserir_arvore(s_arvore *raiz, char palavra[20]) // Função que insere um novo Nó - Utiliza a função auxiliar NovoNo
{
    // Passo 1 - Executa a inserção normal
    if (raiz == NULL) // Se a raiz é nula(Vazia)
    {
        s_arvore *raiz = (s_arvore *)malloc(sizeof(s_arvore)); // Aloca espaço na memória para o registro
        strcpy(raiz->palavra, palavra);                        // palavra de raiz recebe a chave informada pelo usuário
        raiz->Esquerda = NULL;                                 // Como o nó é novo -> Esquerda e -> Direita apontam para nulo
        raiz->Direita = NULL;
        raiz->altura = 0; // O novo nó inserido é inicialmente uma folha portanto sua altura é 1
    }
    else if (raiz != NULL) // Se a raiz não for nula(Vazia)
    {
        if (strcmp(palavra, raiz->palavra) < 0) // Verifica se a chave informada pelo usuário é menor que a chave da raiz
        {
            raiz->Esquerda = inserir_arvore(raiz->Esquerda, palavra); // se for menor, chama novamente a função(recursividade), fazendo a última chave(esquerda) apontar para a nova chave
        }
        // Senão (veja linha abaixo)
        if (strcmp(palavra, raiz->palavra) > 0) // Verifica se a chave informada pelo usuário é maior que a chave da raiz
        {
            raiz->Direita = inserir_arvore(raiz->Direita, palavra); // senão for menor, é maior e chama novamente a função(recursividade), fazendo a última chave(direita) apontar para a nova chave
        }

        // Passo 2 - Atualiza a altura do novo nó (em relação ao anterior)
        raiz->altura = Maior(altura(raiz->Esquerda), altura(raiz->Direita)) + 1;

        // Passo 3 - Declarada uma váriável inteira que tem por objetivo obter o fator de balanceamento deste nó em relação ao anterior(pai) para saber se a arvore ficou desbalanceada
        int fb = Balanceamento(raiz); // Verifica o fator de balanceamento

        // Se o novo nó causou desbalanceamento da árvore, será necessário obter o balanceamento por meio de uma das 4 formas (dependendo do caso)

        // Rotação simples à esquerda
        if (fb > 1 && palavra < raiz->Esquerda->palavra) // Se o fator de balanceamento da raiz for maior que 1 e o palavra informado pelo usuário for menor que o palavra que está na raiz -> Esquerda
            return RotDir(raiz);                         // Retorna a raiz, depois de aplicada a Rotação à Direita(Função)

        // Rotação simples à direita
        if (fb < -1 && palavra > raiz->Direita->palavra)
            return RotEsq(raiz);

        // Rotação dupla à esquerda
        if (fb > 1 && palavra > raiz->Esquerda->palavra)
        {
            raiz->Esquerda = RotEsq(raiz->Esquerda);
            return RotDir(raiz);
        }

        // Rotação dupla à direita
        if (fb < -1 && palavra < raiz->Direita->palavra)
        {
            raiz->Direita = RotDir(raiz->Direita);
            return RotEsq(raiz);
        } // Fim das rotações

        // Passo 4 - Retorna a nova raiz(alterada)
        return raiz;
    }
}

void in_ordem(s_arvore *NoAtual) // Varredura e-r-d (= inorder traversal = percurso em in-ordem) Ordem raiz-Esquerda_Direita.
{
    if (NoAtual == (s_arvore *)NULL) // Se o ponteiro NoAtual do tipo s_arvore(registro) for nulo
        return;                      // Retorna nulo
    // Senão
    in_ordem(NoAtual->Esquerda);     // Senão chama novamente a função indo para a esquerda
    printf("%s ", NoAtual->palavra); // Imprimindo o palavra
    in_ordem(NoAtual->Direita);      // E depois chamando novamente a função indo para a direita
}

