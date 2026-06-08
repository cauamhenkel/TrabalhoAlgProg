#ifndef MONSTRO_H
#define MONSTRO_H

#include "funcoesGerais.h"

typedef struct{
    int posX, velX;
    int posY;
    float animacaoTimer; int frameAtual;
    Direcao dir;
    EstadoObjeto estadoMonstro;
} MONSTRO;

void iniciaMonstros(MONSTRO monstros[M_QTD_MAX], char mapa[TILES][TILES], int *qtdMonstros);
void regulaMovimentoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, char mapa[TILES][TILES]);
void moveMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros);

int temChaoNaDireita(MONSTRO monstro, char mapa[TILES][TILES]);
int temChaoNaEsquerda(MONSTRO monstro, char mapa[TILES][TILES]);

#endif
