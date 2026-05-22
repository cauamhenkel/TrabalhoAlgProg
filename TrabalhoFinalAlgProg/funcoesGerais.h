#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FPS 60
#define LARGURA 600
#define ALTURA 600
#define CABECALHO 50
#define COMP_LINHA 20
#define COMP_COLUNA 20
#define LARG_BOTOES 160
#define ALT_BOTOES 60
#define FONTE_BOTOES 20

typedef enum {
    MENU,
    RANKING,
    EM_JOGO,
    PAUSADO,
    VITORIA,
    DERROTA
} EstadoJogo;

typedef enum {
    MENU_JOGAR,
    MENU_RANKING,
    MENU_SAIR
} EstadoMenu;

typedef enum {
    PAUSE_CONTINUAR,
    PAUSE_VOLTAR_AO_MENU,
    PAUSE_SAIR
} EstadoPausado;

typedef struct tipo_placar
{
char nome[20];
int pontos;
} TIPO_PLACAR;

void reiniciaFase(int *iniciouMapa, int *iniciouPlayer, int *iniciouMonstros);
void criaMapa(char mapa[30][30], int fase, EstadoJogo *estado);
void corrigeMapa(char mapa[30][30]);
void desenhaMapa(char mapa[30][30]);
void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]);
void exibeFase(int fase);

#endif
