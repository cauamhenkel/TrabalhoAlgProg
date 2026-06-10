#ifndef MONSTRO_H
#define MONSTRO_H

#include "funcoesGerais.h"

/* Estrutura do monstro */
typedef struct{
    int posX, velX;
    int posY;
    int frameAtual; float animacaoTimer;
    Direcao dir;
    EstadoObjeto estadoMonstro;
} MONSTRO;

/* Funções que lidam com o monstro no mapa */
void iniciaMonstros(MONSTRO monstros[M_QTD_MAX], char mapa[TILES][TILES], int *qtdMonstros);
void regulaMovimentoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, char mapa[TILES][TILES]);
void moveMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros);

/* Funções booleanas de suporte */
int temChaoNaDireita(MONSTRO monstro, char mapa[TILES][TILES]);
int temChaoNaEsquerda(MONSTRO monstro, char mapa[TILES][TILES]);

#endif

