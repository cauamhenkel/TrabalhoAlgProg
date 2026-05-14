#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGURA 600
#define ALTURA 600
#define CABECALHO 50
#define COMP_LINHA 20
#define COMP_COLUNA 20
#define FPS 60

typedef enum {
    EM_JOGO,
    PAUSADO,
    MENU
} EstadoJogo;

void arqMapa(char nomeMapa[12], int numMapa);
void criaMapa(char mapa[30][30], char *arq);
void desenhaMapa(char mapa[30][30]);

#endif
