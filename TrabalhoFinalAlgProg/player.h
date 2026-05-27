#ifndef PLAYER_H
#define PLAYER_H

#include "funcoesGerais.h"
#include "monstro.h"

typedef struct{
    int posX; float velX, accX;
    int posY; float velY, accY;
    Direcao dir;
    int saude, invencibilidade;
    int naEscada, afetadoGravidade;
    int qtdTiros, cooldown;
} PLAYER;

void iniciaPlayer(PLAYER *p, char mapa[TILES][TILES]);
void desenhaPlayer(PLAYER p, Texture2D sprite);
void exibeSaude(PLAYER p);
void exibeQtdTiros(PLAYER p);
void controlaGravidadePlayer(PLAYER *p, char mapa[TILES][TILES]);
void processaGravidadePlayer(PLAYER *p);
void processaAceleracaoPlayer(PLAYER *p);
void processaAtritoPlayer(PLAYER *p);
void corrigePersonagemY(PLAYER *p);
void centralizaPlayerNaEscada(PLAYER *p, char mapa[TILES][TILES]);
void danoPlayer(PLAYER *p);
void processaMovimentoPlayer(PLAYER *p, char mapa[TILES][TILES]);
void processaPlayerNaEscada(PLAYER *p, char mapa[TILES][TILES]);
void processaPuloPlayer(PLAYER *p, char mapa[TILES][TILES]);
void processaColisoesPlayer(PLAYER *p, char mapa[TILES][TILES]);

int colidiuBordaEsquerda(PLAYER p);
int colidiuBordaDireita(PLAYER p);
int colidiuMonstro(PLAYER p, MONSTRO monstros[10], int qtdMonstros);
int colidiuTeto(PLAYER p, char mapa[TILES][TILES]);
int colidiuParedeDireita(PLAYER p, char mapa[TILES][TILES]);
int colidiuParedeEsquerda(PLAYER p, char mapa[TILES][TILES]);
int playerNoChao(PLAYER p, char mapa[TILES][TILES]);
int playerNaPlataforma(PLAYER p, char mapa[TILES][TILES]);
int checaPlayerMapa(PLAYER p, char mapa[TILES][TILES], char ch);
int playerNaSubida(PLAYER p, char mapa[TILES][TILES]);
int playerNaEscada(PLAYER p, char mapa[TILES][TILES]);
int playerNaDescida(PLAYER p, char mapa[TILES][TILES]);
int playerNaEscadaComPlataforma(PLAYER p, char mapa[TILES][TILES]);
int playerNoFinal(PLAYER p, char mapa[TILES][TILES]);
int caiuDoMapa(PLAYER p);
int pixelDeCimaVazio(PLAYER p, char mapa[TILES][TILES]);

#endif
