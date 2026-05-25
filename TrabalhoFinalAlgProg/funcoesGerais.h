#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CONSTANTES GERIAS */
#define FPS 60

#define TILES 30
#define COMP_LINHA 30
#define COMP_COLUNA 20

#define LARGURA (COMP_LINHA*TILES) // Define a largura da tela
#define ALTURA (COMP_COLUNA*TILES) // Define a altura da tela
#define CABECALHO (COMP_COLUNA*2)

#define LARG_BOTOES (COMP_LINHA*8)
#define ALT_BOTOES (COMP_COLUNA*3)

#define FONTE_GERAL (COMP_LINHA*3)
#define FONTE_BOTOES (COMP_LINHA)
#define FONTE_CABECALHO (FONTE_BOTOES * 1.3)

/* CONSTANTES FISICAS */
#define GRAVIDADE (COMP_COLUNA / 22.0f) // Define a aceleracao gravitacional
#define FORCA_ATRITO (P_ACC_X / 1.5f) // Define a forca de atrito

/* CONSTANTES PLAYER */
#define P_VEL_X_MAX (COMP_LINHA/4)
#define P_VEL_Y_MAX (COMP_COLUNA)
#define P_ACC_X (P_VEL_X_MAX / 8.0f)
#define P_VEL_ESCADA (COMP_COLUNA/5)
#define P_VEL_PULO -(COMP_COLUNA/2)

/* CONSTANTES MONSTRO */
#define M_QTD_MAX 100
#define M_VEL_X (COMP_LINHA/8)

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

typedef struct tipo_placar{
    char nome[20];
    int pontos;
} TIPO_PLACAR;

void reiniciaFase(int *iniciouMapa, int *iniciouPlayer, int *iniciouMonstros);
void criaMapa(char mapa[TILES][TILES], int fase, EstadoJogo *estado);
void corrigeMapa(char mapa[TILES][TILES]);
void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]);
void exibeFase(int fase);

#endif
