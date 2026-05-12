#include "funcoesGerais.h"

typedef struct{
    int posX; int velX;
    int posY;
} MONSTRO;

void iniciaMonstros(MONSTRO monstros[10], char mapa[30][30]);
void moveMonstros(MONSTRO monstros[10], int qtdMonstros);
void desenhaMonstros(MONSTRO monstros[10], int qtdMonstros);

int calculaQtdMonstros(char mapa[30][30]);
