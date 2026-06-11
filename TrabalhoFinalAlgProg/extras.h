#ifndef EXTRAS_H
#define EXTRAS_H

#include "raylib.h"
#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"
#include "graficos.h"

/* Estrutura do projetil */
typedef struct{
    int posX; float velX;
    int posY;
    EstadoObjeto estado;
} PROJETIL;

/* Funções relacionadas do projetil */
void criaProjetil(PLAYER p, PROJETIL *pr);
void moveProjetil(PROJETIL *pr);
void processaColisoesProjetil(PROJETIL *pr, char mapa[TILES][TILES]);
void processaProjetil(PLAYER *p, PROJETIL *pr, char mapa[TILES][TILES], Soundtrack sounds);
void desenhaPontosSubindo(Vector2 vec);
void desenhaProjetil(PROJETIL pr);

Vector2 mataMonstrosProjetil(PLAYER *p, PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros, int *qtdMonstrosMortos, int *pontos);

#endif

