#ifndef GRAFICOS_H
#define GRAFICOS_H

#include "funcoesGerais.h"
#include "player.h"
#include "raylib.h"

#define TAM_TILE 32

typedef struct{
    Texture2D player;
    Texture2D monstro;
    Texture2D tileset;
} Spritesheet;

typedef struct {
    Sound botao;
    Sound caminhada;
    Sound dano;
    Sound gameover;
    Sound mordida1;
    Sound mordida2;
    Sound salto1;
    Sound salto2;
    Sound monstro_dano;
    Sound tiro;
} Soundtrack;

void carregaSpritesheet(Spritesheet *sprites);
void descarregaSpritesheet(Spritesheet *sprites);
void carregaSoundtrack(Soundtrack *sounds);
void descarregaSoundtrack(Soundtrack *sounds);
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
