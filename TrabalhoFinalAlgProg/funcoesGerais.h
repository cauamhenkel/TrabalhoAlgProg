#pragma once
// garante que o compilador so inclua esse arquivo uma vez

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LARGURA 600
#define ALTURA 600
#define CABECALHO 50
#define COMP_LINHA 20
#define COMP_COLUNA 20

typedef enum {
    EM_JOGO,
    PAUSADO,
    MENU
} EstadoJogo;

void arqMapa(char nomeMapa[12], const char *nMapa);
void criaMapa(char mapa[30][30], char *arq);
void desenhaMapa(char mapa[30][30]);
