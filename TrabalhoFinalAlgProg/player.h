#include "funcoesGerais.h"
#include "monstro.h"

typedef struct{
    int posX; int velX;
    int posY; int velY; int accY;
    int saude; int invencibilidade;
} PLAYER;

void iniciaPlayer(PLAYER *p, char mapa[30][30]);
void desenhaPlayer(PLAYER p);
void exibeSaude(PLAYER p);
void gravidade(PLAYER *p, int *terco);
void corrigePersonagem(PLAYER *p);
void danoPlayer(PLAYER *p);

int colidiuBordaEsquerda(PLAYER p);
int colidiuBordaDireita(PLAYER p);
int colidiuMonstro(PLAYER p, MONSTRO monstros[10], int qtdMonstros);
int colidiuTeto(PLAYER p, char mapa[30][30]);
int colidiuParedeDireita(PLAYER p, char mapa[30][30]);
int colidiuParedeEsquerda(PLAYER p, char mapa[30][30]);
int checaPlayerMapa(PLAYER p, char mapa[30][30], char ch);
int playerNoChao(PLAYER p, char mapa[30][30]);
int playerNaSubida(PLAYER p, char mapa[30][30]);
int playerNaEscada(PLAYER p, char mapa[30][30]);
int playerNaDescida(PLAYER p, char mapa[30][30]);
int playerNoFinal(PLAYER p, char mapa[30][30]);
int caiuDoMapa(PLAYER p);
int pixelDeCimaVazio(PLAYER p, char mapa[30][30]);
