#ifndef GRAFICOS_H
#define GRAFICOS_H

#include "raylib.h"
#include "funcoesGerais.h"
#include "player.h"

#define TAM_TILE 32

/* Estrutura contendo os sprites */
typedef struct{
    Texture2D player;
    Texture2D monstro;
    Texture2D tileset;
    Texture2D hud_saude;
    Texture2D hud_tiro;
    Texture2D background;
} Spritesheet;
/* Estrutura contendo os sons */
typedef struct {
    Sound botao;
    Sound dano;
    Sound salto;
    Sound monstro_dano;
    Sound tiro;
} Soundtrack;
/* Funções carregando e descarregando coisas da memória */
void carregaSpritesheet(Spritesheet *sprites);
void descarregaSpritesheet(Spritesheet *sprites);
void carregaSoundtrack(Soundtrack *sounds);
void descarregaSoundtrack(Soundtrack *sounds);
/* Funções desenhando os elementos do jogo*/
void desenhaBackground(Spritesheet sprites);
void desenhaMapa(char mapa[TILES][TILES], Texture2D tileset);
void exibeSaude(PLAYER p, Spritesheet sprites);
void exibeCabecalho(PLAYER p, Spritesheet sprites, int fase, int pontos);
void atualizaAnimacaoPlayer(PLAYER *p);
void desenhaPlayer(PLAYER p, Texture2D sprite);
void atualizaAnimacaoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros);
void desenhaMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, Texture2D sprite);
/* Funções de apoio para as funções de desenho */
Rectangle selecionaTile(int coluna, int linha, int tam_tile);
Rectangle selecionaTileInverso(int coluna, int linha, int tam_tile);
Rectangle selecionaFramePlayer(PLAYER p);
Rectangle selecionaFrameMonstro(MONSTRO monstro);

#endif
