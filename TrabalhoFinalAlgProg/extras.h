#ifndef EXTRAS_H
#define EXTRAS_H

#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"

#define PROJ_VEL_X COMP_LINHA

typedef struct{
    int posX; float velX;
    int posY;
    EstadoObjeto estado;
} PROJETIL;

void criaProjetil(PLAYER p, PROJETIL *pr);
void moveProjetil(PROJETIL *pr);
void processaColisoesProjetil(PROJETIL *pr, MONSTRO monstros[M_QTD_MAX], char mapa[TILES][TILES]);
void mataMonstrosProjetil(PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros);
void desenhaProjetil(PROJETIL pr);

#endif
