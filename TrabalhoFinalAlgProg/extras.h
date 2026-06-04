#ifndef EXTRAS_H
#define EXTRAS_H

#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"

typedef struct{
    int posX; float velX;
    int posY;
    EstadoObjeto estado;
} PROJETIL;

void criaProjetil(PLAYER p, PROJETIL *pr);
void moveProjetil(PROJETIL *pr);
void processaColisoesProjetil(PROJETIL *pr, char mapa[TILES][TILES]);
void processaProjetil(PLAYER *p, PROJETIL *pr, char mapa[TILES][TILES]);
void mataMonstrosProjetil(PLAYER *p, PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros, int *pontos);
void desenhaProjetil(PROJETIL pr);

#endif
