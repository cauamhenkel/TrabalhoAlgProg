#pragma once
// garante que o compilador so inclua esse arquivo uma vez

#include "funcoesGerais.h"
#include "monstro.h"

typedef struct{
    int posX; int velX;
    int posY; int velY; int accY;
    int saude;
    int invencibilidade;
} PLAYER;

void iniciaPlayer(PLAYER *p, char mapa[30][30]);
void desenhaPlayer(PLAYER p);
void sobeEscada(PLAYER *p, char mapa[30][30]);
void desceEscada(PLAYER *p, char mapa[30][30]);
void gravidade(PLAYER *p, int *terco);
void corrigePersonagem(PLAYER *p);

int colidiuBordaEsquerda(PLAYER p);
int colidiuBordaDireita(PLAYER p);
int colidiuMonstro(PLAYER p, MONSTRO monstros[10], int qtdMonstros);
int checaPlayerMapa(PLAYER p, char mapa[30][30], char ch);
int playerNoChao(PLAYER p, char mapa[30][30]);
int playerNaEscada(PLAYER p, char mapa[30][30]);
int playerNaDescida(PLAYER p, char mapa[30][30]);
int playerNoFinal(PLAYER p, char mapa[30][30]);
int caiuDoMapa(PLAYER p);

void exibeSaude(PLAYER p);
void danoPlayer(PLAYER *p);
void matarPlayer(PLAYER *p, EstadoJogo *ej, int posX0, int posY0);
