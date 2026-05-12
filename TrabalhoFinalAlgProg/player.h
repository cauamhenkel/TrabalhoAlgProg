#include "funcoesGerais.h"

typedef struct{
    int posX; int velX;
    int posY; int velY; int accY;
    int saude;
} PLAYER;

void iniciaPlayer(PLAYER *p, char mapa[30][30]);
void desenhaPlayer(PLAYER p);
void sobeEscada(PLAYER *p, char mapa[30][30]);
void desceEscada(PLAYER *p, char mapa[30][30]);
void gravidade(PLAYER *p, int *terco);
void corrigePersonagem(PLAYER *p);

int colidiuBordaEsquerda(PLAYER p);
int colidiuBordaDireita(PLAYER p);
int checaPlayerMapa(PLAYER p, char mapa[30][30], char ch);
int playerNoChao(PLAYER p, char mapa[30][30]);
int playerNaEscada(PLAYER p, char mapa[30][30]);
int playerNaDescida(PLAYER p, char mapa[30][30]);
