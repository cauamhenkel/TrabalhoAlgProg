#ifndef EXTRAS_H
#define EXTRAS_H

#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"
#include "raylib.h"
#include "graficos.h"

typedef struct{
    int posX; float velX;
    int posY;
    EstadoObjeto estado;
} PROJETIL;

void criaProjetil(PLAYER p, PROJETIL *pr);
void moveProjetil(PROJETIL *pr);
void processaColisoesProjetil(PROJETIL *pr, char mapa[TILES][TILES]);
void processaProjetil(PLAYER *p, PROJETIL *pr, char mapa[TILES][TILES], Soundtrack sounds);
Vector2 mataMonstrosProjetil(PLAYER *p, PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros, int *pontos);
void desenhaPontosSubindo(Vector2 vec);
void desenhaProjetil(PROJETIL pr);

#endif
