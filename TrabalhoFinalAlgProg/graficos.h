#ifndef GRAFICOS_H
#define GRAFICOS_H

#include "funcoesGerais.h"
#include "player.h"

#define TAM_TILE 32

typedef struct{
    Texture2D player;
    Texture2D monstro;
    Texture2D tileset;
} Spritesheet;

void carregaSpritesheet(Spritesheet *sprites);
void descarregaSpritesheet(Spritesheet *sprites);
void desenhaMapa(char mapa[TILES][TILES], Texture2D tileset);
void exibeCabecalho(PLAYER p, int fase, int pontos);
void atualizaAnimacaoPlayer(PLAYER *p);
void desenhaPlayer(PLAYER p, Texture2D sprite);
void atualizaAnimacaoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros);
void desenhaMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, Texture2D sprite);

Rectangle selecionaTile(int coluna, int linha, int tam_tile);
Rectangle selecionaTileInverso(int coluna, int linha, int tam_tile);
Rectangle selecionaFramePlayer(PLAYER p);
Rectangle selecionaFrameMonstro(MONSTRO monstro);

#endif
