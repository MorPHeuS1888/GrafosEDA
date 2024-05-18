#include <stdio.h>
#include <stdlib.h>
#ifndef GRAFOS_H
#define GRAFOS_H

struct dados_vertice
{
    int valor;
    int coluna;
    int linha;
};

struct arco
{
    struct vertice* destino;
    struct arco* proximo;
};

struct vertice
{
    struct dados_vertice* dados;
    struct vertice* proximo;
    struct arco* arcos;
};

void listagemSimples(struct vertice* matriz);

int adjacente(struct vertice* v1, struct vertice* v2);

struct vertice* procura_vertice(struct vertice* grafo, struct dados_vertice* dados);

void juntar(struct vertice* grafo, struct dados_vertice* v1, struct dados_vertice* v2);

void limpaMemoria(struct vertice* lista);

struct vertice* verticePorPosicao(struct vertice* matriz, int linha, int coluna);

struct vertice* insereMatriz(struct vertice* matriz, int valor, int linha, int coluna);

int obtemMaiorValor(struct vertice* grafo);

struct vertice* verticeDaDireita(struct vertice* vertice);

struct vertice* verticeDaEsquerda(struct vertice* vertice);

struct vertice* verticeDeCima(struct vertice* vertice);

struct vertice* verticeDeBaixo(struct vertice* vertice);

struct vertice* procuraElementoNaPosicao(struct vertice* matriz, int linha, int coluna);

void alterarValorVertice(struct vertice* matriz, int linha, int coluna, int valor);

struct vertice* insereLinhaMatriz(struct vertice* matriz, int linhaInserir, int numeroColunas);

struct vertice* insereColunaMatriz(struct vertice* matriz, int colunaInserir, int numeroLinhas);

struct vertice* removeColunaMatriz(struct vertice* matriz, int colunaRemover);

struct vertice* removeLinhaMatriz(struct vertice* matriz, int linhaRemover);

void imprimeLista(struct vertice* lista);

int somaElementos(struct vertice* lista);

int contaElementos(struct vertice* lista);

struct vertice* matrizRestante(struct vertice* matriz, int linha, int coluna);

#endif 