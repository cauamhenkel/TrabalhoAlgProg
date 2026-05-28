#ifndef GRAFICOS_H
#define GRAFICOS_H

#include "funcoesGerais.h"
#include "player.h"

#define TAM_TILE 32

typedef struct {
    Texture2D player;
    Texture2D monstro;
    Texture2D tileset;
} Spritesheet;

void carregaSpritesheet(Spritesheet *sprites);
void descarregaSpritesheet(Spritesheet *sprites);
void desenhaMapa(char mapa[TILES][TILES], Texture2D tileset);
void exibeCabecalho(PLAYER p, int fase, int pontos);

Rectangle selecionaTile(int coluna, int linha);

#endif
