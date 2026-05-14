#ifndef MONSTRO_H
#define MONSTRO_H

#include "funcoesGerais.h"

typedef struct{
    int posX; int velX;
    int posY;
} MONSTRO;

void iniciaMonstros(MONSTRO monstros[10], char mapa[30][30], int *qtdMonstros);
void regulaMovimentoMonstros(MONSTRO monstros[10], int qtdMonstros, char mapa[30][30]);
void moveMonstros(MONSTRO monstros[10], int qtdMonstros);
void desenhaMonstros(MONSTRO monstros[10], int qtdMonstros);

int temChaoNaDireita(MONSTRO monstro, char mapa[30][30]);
int temChaoNaEsquerda(MONSTRO monstro, char mapa[30][30]);

#endif
