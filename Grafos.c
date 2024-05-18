#include <stdlib.h>
#include <stdio.h>
#include "Grafos.h"

void listagemSimples(struct vertice* matriz)
{
    struct vertice* currentElemento = matriz;
    while (currentElemento != NULL)
    {
        printf("[%d (%d, %d)] ", currentElemento->dados->valor, currentElemento->dados->linha, currentElemento->dados->coluna);
        currentElemento = currentElemento->proximo;
    }
    printf("\n");
}

int adjacente(struct vertice* v1, struct vertice* v2)
{
    struct arco* ap = v1->arcos;
    while (ap != NULL && ap->destino != v2)
    {
        ap = ap->proximo;
    }
	return (ap != NULL);
}

struct vertice* procura_vertice(struct vertice* grafo, struct dados_vertice* dados)
{
    struct vertice* ap = grafo;
    while (ap != NULL && ap->dados != dados)
    {
        ap = ap->proximo;
    }
    if (ap == NULL)
    {
        struct vertice* novo = malloc(sizeof(struct vertice));
        novo->dados = dados;
        novo->proximo = grafo;
        novo->arcos = NULL;
        ap = novo;
    }
    return ap;
}

void juntar(struct vertice* grafo, struct dados_vertice* v1, struct dados_vertice* v2)
{
    struct vertice* apVertice = procura_vertice(grafo, v1);
    struct arco* apArco = apVertice->arcos;

    while (apArco != NULL && apArco->destino->dados != v2)
    {
        apArco = apArco->proximo;
    }
    if (apArco == NULL)
    {
        // se chegou ao fim sem encontrar o arco entre v1 e v2, cria v1->v2 e v2->v1
        struct arco* novo1 = malloc(sizeof(struct arco));
        novo1->destino = procura_vertice(grafo, v2);
        novo1->proximo = apVertice->arcos;
        apVertice->arcos = novo1;

        struct arco* novo2 = malloc(sizeof(struct arco));
        novo2->destino = apVertice;
        novo2->proximo = novo1->destino->arcos;
        novo1->destino->arcos = novo2;
    }
}

void juntarVertices(struct vertice* v1, struct vertice* v2)
{
    if (v1 == NULL || v2 == NULL)
        return;

    //printf("Juntar %d (%d, %d) com %d (%d, %d) e vice-versa\n", 
    //                v1->dados->valor, v1->dados->linha, v1->dados->coluna,
    //                v2->dados->valor, v2->dados->linha, v2->dados->coluna);

    struct arco* apArco = v1->arcos;
    
    while (apArco != NULL && apArco->destino != v2)
    {
        apArco = apArco->proximo;
    }
    
    if (apArco == NULL)
    {
        // se chegou ao fim sem encontrar o arco entre v1 e v2, cria v1->v2 e v2-> v1
        struct arco* novo1 = malloc(sizeof(struct arco));
        novo1->destino = v2;
        novo1->proximo = v1->arcos;
        v1->arcos = novo1;        

        struct arco* novo2 = malloc(sizeof(struct arco));
        novo2->destino = v1;
        novo2->proximo = v2->arcos;
        v2->arcos = novo2;
    }
}

void separar(struct vertice* v1, struct vertice* v2)
{
    struct arco* arcoV1 = v1->arcos;
    struct arco* arcoAnterior = NULL;

    while (arcoV1 != NULL && arcoV1->destino != v2)
    {
        arcoAnterior = arcoV1;
        arcoV1 = arcoV1->proximo;
    }
    if (arcoV1 != NULL)
    {
        if (arcoV1 == v1->arcos)
            v1->arcos = arcoV1->proximo;
        else
            arcoAnterior->proximo = arcoV1->proximo;

        free(arcoV1);
    }
}

void removeLigacoes(struct vertice* v1, struct vertice* v2)
{
    if (v1 != NULL && v2 != NULL)
    {
        //printf("Separar %d (%d, %d) de %d (%d, %d) e vice-versa\n", 
        //            v1->dados->valor, v1->dados->linha, v1->dados->coluna,
        //            v2->dados->valor, v2->dados->linha, v2->dados->coluna);
        separar(v1, v2);
        separar(v2, v1);
    }
}

void limpaMemoria(struct vertice* lista)
{
    struct vertice* currentElemento = lista;
    while (currentElemento != NULL)
    {
        struct vertice* eliminar = currentElemento;
        currentElemento = currentElemento->proximo;

        // limpar arcos
        struct arco* currentArco = eliminar->arcos;
        while (currentArco != NULL)
        {
            struct arco* eliminarArco = currentArco;
            currentArco = currentArco->proximo;
            free(eliminarArco);
        }
        free(eliminar->dados);
        free(eliminar);
    }
}

struct vertice* criaVertice(int valor, int linha, int coluna)
{
    struct dados_vertice* dados = malloc(sizeof(struct dados_vertice));
    struct vertice* novo = malloc(sizeof(struct vertice));
    dados->valor = valor;
    dados->linha = linha;
    dados->coluna = coluna;
    novo->dados = dados;
    novo->arcos = NULL;
    novo->proximo = NULL;
    //printf("Criado vertice com valor %d\n", novo->dados->valor);
    return novo;
}

struct vertice* verticePorPosicao(struct vertice* matriz, int linha, int coluna)
{
    if (linha < 0)
        return NULL;
    if (coluna < 0)
        return NULL;
    struct vertice* ap = matriz;
    
    while (ap != NULL && (ap->dados->linha != linha || ap->dados->coluna != coluna))
    {        
        ap = ap->proximo;
    }
    return ap;
}

struct vertice* insereMatriz(struct vertice* matriz, int valor, int linha, int coluna)
{
    struct vertice* novo = criaVertice(valor, linha, coluna);
    novo->proximo = matriz;
    matriz = novo;
    struct vertice* cima = verticePorPosicao(matriz, linha - 1, coluna);
    struct vertice* baixo = verticePorPosicao(matriz, linha + 1, coluna);
    struct vertice* esquerda = verticePorPosicao(matriz, linha, coluna - 1);
    struct vertice* direita = verticePorPosicao(matriz, linha, coluna + 1);

    if (cima != NULL)
        juntarVertices(novo, cima);
    if (baixo != NULL)
        juntarVertices(novo, baixo);
    if (esquerda != NULL)
        juntarVertices(novo, esquerda);
    if (direita != NULL)
        juntarVertices(novo, direita);

    return novo;
}

int obtemMaiorValor(struct vertice* grafo) 
{
    struct vertice* currentElemento = grafo;
    int maior = 0;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->valor > maior)
        {
            maior = currentElemento->dados->valor;
        }
        currentElemento = currentElemento->proximo;
    }
    return maior;
}

struct vertice* verticeDaDireita(struct vertice* vertice)
{
    struct arco* apArco = vertice->arcos;
    while (apArco != NULL)
    {
        if (apArco->destino->dados->coluna > vertice->dados->coluna)
            break;
        apArco = apArco->proximo;        
    }

    if (apArco != NULL)
    {
        return apArco->destino;
    }
    else
    {
        return NULL;
    }
}

struct vertice* verticeDaEsquerda(struct vertice* vertice)
{
    struct arco* apArco = vertice->arcos;
    while (apArco != NULL && apArco->destino->dados->coluna >= vertice->dados->coluna)
    {
        apArco = apArco->proximo;
    }
    if (apArco != NULL)
    {
        return apArco->destino;
    }
    else
    {
        return NULL;
    }
}

struct vertice* verticeDeCima(struct vertice* vertice)
{
    struct arco* apArco = vertice->arcos;
    while (apArco != NULL && apArco->destino->dados->linha >= vertice->dados->linha)
    {
        apArco = apArco->proximo;
    }
    if (apArco != NULL)
    {
        return apArco->destino;
    }
    else
    {
        return NULL;
    }
}

struct vertice* verticeDeBaixo(struct vertice* vertice)
{
    struct arco* apArco = vertice->arcos;
    while (apArco != NULL && apArco->destino->dados->linha <= vertice->dados->linha)
    {
        apArco = apArco->proximo;
    }
    if (apArco != NULL)
    {
        return apArco->destino;
    }
    else
    {
        return NULL;
    }
}

struct vertice* procuraElementoNaPosicao(struct vertice* matriz, int linha, int coluna)
{
    struct vertice* currentElemento = matriz;
    int linhaAtual = 0;
    int colunaAtual = 0;
    if (currentElemento != NULL)
    {
        linhaAtual = currentElemento->dados->linha;
        colunaAtual = currentElemento->dados->coluna;
    }

    // anda para baixo
    while (currentElemento != NULL && linhaAtual < linha)
    {
        currentElemento = verticeDeBaixo(currentElemento);
        if (currentElemento != NULL)
        {
            linhaAtual = currentElemento->dados->linha;
            colunaAtual = currentElemento->dados->coluna;
        }
    }
    // anda para cima
    while (currentElemento != NULL && linhaAtual > linha)
    {
        currentElemento = verticeDeCima(currentElemento);
        if (currentElemento != NULL)
        {
            linhaAtual = currentElemento->dados->linha;
            colunaAtual = currentElemento->dados->coluna;
        }
    }
    // anda para a esquerda
    while (currentElemento != NULL && colunaAtual > coluna)
    {
        currentElemento = verticeDaEsquerda(currentElemento);
        if (currentElemento != NULL)
        {
            linhaAtual = currentElemento->dados->linha;
            colunaAtual = currentElemento->dados->coluna;
        }
    }
    // anda para a direita
    while (currentElemento != NULL && colunaAtual < coluna)
    {
        currentElemento = verticeDaDireita(currentElemento);
        if (currentElemento != NULL)
        {
            linhaAtual = currentElemento->dados->linha;
            colunaAtual = currentElemento->dados->coluna;
        }
    }

    return currentElemento;
}

void alterarValorVertice(struct vertice* matriz, int linha, int coluna, int valor) 
{
    struct vertice* elemento = procuraElementoNaPosicao(matriz, linha, coluna);
    if (elemento != NULL)
        elemento->dados->valor = valor;
}

struct vertice* insereLinhaMatriz(struct vertice* matriz, int linhaInserir, int numeroColunas)
{
    // fazer um ciclo para correr a matriz toda
    struct vertice* currentElemento = matriz;
    while (currentElemento != NULL)
    {
        // aumentar a linha dos elementos na linha igual ou acima da que queremos inserir
        if (currentElemento->dados->linha >= linhaInserir)
            currentElemento->dados->linha = currentElemento->dados->linha + 1;

        currentElemento = currentElemento->proximo;
    }
    // remover ligacoes 
    currentElemento = matriz;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->linha == linhaInserir + 1)
        {
            int coluna = currentElemento->dados->coluna;
            int linha = currentElemento->dados->linha;
            struct vertice* outroElemento = procuraElementoNaPosicao(matriz, linha - 2, coluna);
            if (outroElemento != NULL)
            {
                separar(currentElemento, outroElemento);
                separar(outroElemento, currentElemento);
            }
        }
        currentElemento = currentElemento->proximo;
    }
    // inserir n elementos e refazer ligações
    for (int i = 0; i < numeroColunas; i++)
    {
        matriz = insereMatriz(matriz, 0, linhaInserir, i);
    }
    return matriz;
}

struct vertice* insereColunaMatriz(struct vertice* matriz, int colunaInserir, int numeroLinhas)
{
    // fazer um ciclo para correr a matriz toda
    struct vertice* currentElemento = matriz;
    while (currentElemento != NULL)
    {
        // aumentar a coluna dos elementos na coluna igual ou acima da que queremos inserir
        if (currentElemento->dados->coluna >= colunaInserir)
            currentElemento->dados->coluna = currentElemento->dados->coluna + 1;

        currentElemento = currentElemento->proximo;
    }
    // remover ligacoes 
    currentElemento = matriz;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->coluna == colunaInserir + 1)
        {
            int coluna = currentElemento->dados->coluna;
            int linha = currentElemento->dados->linha;
            struct vertice* outroElemento = procuraElementoNaPosicao(matriz, linha, coluna - 2);
            if (outroElemento != NULL)
            {
                separar(currentElemento, outroElemento);
                separar(outroElemento, currentElemento);
            }
        }
        currentElemento = currentElemento->proximo;
    }
    // inserir n elementos e refazer ligações
    for (int i = 0; i < numeroLinhas; i++)
    {
        matriz = insereMatriz(matriz, 0, i, colunaInserir);
    }
    return matriz;
}

struct vertice* removeColunaMatriz(struct vertice* matriz, int colunaRemover)
{
    struct vertice* novaMatriz = matriz;
    // fazer um ciclo para correr a matriz toda
    struct vertice* currentElemento = matriz;
    struct vertice* anteriorElemento = NULL;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->coluna == colunaRemover)
        {
            if (currentElemento == novaMatriz)
                novaMatriz = novaMatriz->proximo;

            // obtem vertices adjacentes
            struct vertice* vE = verticeDaEsquerda(currentElemento);
            struct vertice* vD = verticeDaDireita(currentElemento);
            struct vertice* vC = verticeDeCima(currentElemento);
            struct vertice* vB = verticeDeBaixo(currentElemento);

            // remove ligacoes
            removeLigacoes(currentElemento, vD);
            removeLigacoes(currentElemento, vE);
            removeLigacoes(currentElemento, vC);
            removeLigacoes(currentElemento, vB);

            free(currentElemento->dados);
            struct vertice* eliminar = currentElemento;
            if (anteriorElemento != NULL)
            {
                anteriorElemento->proximo = currentElemento->proximo;
            }
            currentElemento = currentElemento->proximo;   
            free(eliminar);

            // juntar vertices de cima com o de baixo e o da esquerda com o da direita
            juntarVertices(vC, vB);
            juntarVertices(vE, vD);
        }
        else
        {
            anteriorElemento = currentElemento;
            currentElemento = currentElemento->proximo;
        }
    }

    // fazer um ciclo para correr a matriz toda e diminuir o numero da coluna
    currentElemento = novaMatriz;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->coluna > colunaRemover)
        {
            currentElemento->dados->coluna--;   
        }         
        currentElemento = currentElemento->proximo;
    }

    return novaMatriz;
}

struct vertice* removeLinhaMatriz(struct vertice* matriz, int linhaRemover)
{
    struct vertice* novaMatriz = matriz;
    // fazer um ciclo para correr a matriz toda e remover os elementos
    struct vertice* currentElemento = matriz;
    struct vertice* anteriorElemento = NULL;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->linha == linhaRemover)
        {
            if (currentElemento == novaMatriz)
                novaMatriz = novaMatriz->proximo;

            // obtem vertices adjacentes
            struct vertice* vE = verticeDaEsquerda(currentElemento);
            struct vertice* vD = verticeDaDireita(currentElemento);
            struct vertice* vC = verticeDeCima(currentElemento);
            struct vertice* vB = verticeDeBaixo(currentElemento);

            // remove ligacoes
            removeLigacoes(currentElemento, vD);
            removeLigacoes(currentElemento, vE);
            removeLigacoes(currentElemento, vC);
            removeLigacoes(currentElemento, vB);

            free(currentElemento->dados);
            struct vertice* eliminar = currentElemento;            
            if (anteriorElemento != NULL)
            {
                anteriorElemento->proximo = currentElemento->proximo;
            }
            currentElemento = currentElemento->proximo;            
            free(eliminar);

            // juntar vertices de cima com o de baixo e o da esquerda com o da direita
            juntarVertices(vC, vB);
            juntarVertices(vE, vD);
        }
        else
        {
            anteriorElemento = currentElemento;
            currentElemento = currentElemento->proximo;
        }
    }

    // fazer um ciclo para correr a matriz toda e diminuir o numero da linha
    currentElemento = novaMatriz;
    while (currentElemento != NULL)
    {
        if (currentElemento->dados->linha > linhaRemover)
        {
            currentElemento->dados->linha--;   
        }         
        currentElemento = currentElemento->proximo;        
    }
    return novaMatriz;
}

void imprimeLista(struct vertice* lista)
{
    int posicao = 0;
    struct vertice* listatemp = lista; 
    while (listatemp != NULL)
    {
        printf("O elemento numero %d e' %d\n", posicao, listatemp->dados->valor); 
        posicao = posicao + 1;
        listatemp = listatemp->proximo;
    }    
}

int somaElementos(struct vertice* lista)
{
    struct vertice* currentElemento = lista;
    int soma = 0;
    while (currentElemento != NULL)
    {
        soma = soma + currentElemento->dados->valor;
        currentElemento = currentElemento->proximo;
    }
    return soma;
}

int contaElementos(struct vertice* lista)
{
    struct vertice* currentElemento = lista;
    int contador = 0;
    while (currentElemento != NULL)
    {
        contador++;
        currentElemento = currentElemento->proximo;
    }
    return contador;
}

struct vertice* matrizRestante(struct vertice* matriz, int linha, int coluna)
{
	struct vertice* resultado = NULL;

    struct vertice* currentElemento = matriz;
    int currentPosition = 0;
    while (currentElemento != NULL)
    {
        // encontrar a linha e coluna de um determinado elemento da matriz 
        int currentLinha = currentElemento->dados->linha;
        int currentColuna = currentElemento->dados->coluna;
        int currentValor = currentElemento->dados->valor;
        // determinar se o elemento deve fazer parte da matriz restante
        if (currentLinha != linha && currentColuna != coluna)
        {
            int linhaInserir = currentLinha;
            if (currentLinha > linha)
                linhaInserir--;
            int colunaInserir = currentColuna;
            if (currentColuna > coluna)
                colunaInserir--;
            resultado = insereMatriz(resultado, currentValor, linhaInserir, colunaInserir);
        }
        currentElemento = currentElemento->proximo;
    }
	return resultado;
}
