#ifndef FUNCOES_GERAIS_H
#define FUNCOES_GERAIS_H

#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* CONSTANTES GERAIS */
#define FPS 60

/* CONSTANTES PARA O TAMANHO DO GRID E DOS ELEMENTOS DO JOGO */
#define TILES 30
#define COMP_LINHA 20
#define COMP_COLUNA 20
/* CONSTANTES PARA O TAMANHO DA JANELA */
#define LARGURA (COMP_LINHA*TILES)
#define ALTURA (COMP_COLUNA*TILES)
#define CABECALHO (COMP_COLUNA*2)
/* CONSTANTES PARA OS BOTÕES */
#define LARG_BOTOES (COMP_LINHA*8)
#define ALT_BOTOES (COMP_COLUNA*3)
#define LARG_BOTOES_RANKING LARG_BOTOES
#define ALT_BOTOES_RANKING ALT_BOTOES*0.75
/* CONSTANTES PARA AS FONTES DO JOGO */
#define FONTE_GERAL (COMP_LINHA*3)
#define FONTE_BOTOES (COMP_LINHA)
#define FONTE_RANKING (COMP_LINHA/2)
#define FONTE_CABECALHO (FONTE_BOTOES * 1.3)
/* CONSTANTES PARA O PLACAR E PONTOS */
#define TAM_NOME_RANKING 20 // Quantidade de caracteres da string
#define TAM_PLACAR 10
#define QTD_PONTOS_INICIAIS 5000
#define QTD_PONTOS_GANHO_DOS_INIMIGOS 500
#define QTD_REDUZIR_PONTOS 200
#define FRAMES_PARA_REDUZIR_PONTOS FPS*2
#define TEMPO_PTS_SUBINDO 1.5f

/* CONSTANTES FISICAS */
#define GRAVIDADE (COMP_COLUNA / 22.0f) // Define a aceleracao gravitacional
#define FORCA_ATRITO (P_ACC_X / 1.5f) // Define a forca de atrito

/* CONSTANTES PLAYER */
#define P_VEL_X_MAX (COMP_LINHA/4)
#define P_VEL_Y_MAX (COMP_COLUNA)
#define P_ACC_X (P_VEL_X_MAX / 8.0f)
#define P_VEL_ESCADA (COMP_COLUNA/5)
#define P_VEL_PULO -(COMP_COLUNA/2)
#define P_VIDA_MAX 3
#define P_QTD_TIROS 3
#define P_COOLDOWN_TIRO FPS

/* CONSTANTES MONSTRO */
#define M_QTD_MAX 20
#define M_VEL_X (COMP_LINHA/8)

/* CONSTANTES PROJETIL*/
#define PROJ_VEL_X COMP_LINHA*0.75

/* Definições de enum para melhor legibilidade */
typedef enum{
    MENU,
    RANKING,
    EM_JOGO,
    PAUSADO,
    VITORIA,
    DERROTA
} EstadoJogo;

typedef enum{
    MENU_JOGAR,
    MENU_RANKING,
    MENU_SAIR
} EstadoMenu;

typedef enum{
    PAUSE_CONTINUAR,
    PAUSE_VOLTAR_AO_MENU,
    PAUSE_SAIR
} EstadoPausado;

typedef enum{
    DIREITA,
    ESQUERDA
} Direcao;

typedef enum{
    ATIVO,
    DESATIVADO
} EstadoObjeto;

typedef enum{
    NORMAL,
    PACIFISTA,
    GENOCIDA
} ModoVitoria;

/* Estrutura do placar */
typedef struct tipo_placar{
    char nome[TAM_NOME_RANKING];
    int pontos;
    ModoVitoria modoVitoria;
} TIPO_PLACAR;

/* Funções para o placar */
void lePlacar(TIPO_PLACAR placar[TAM_PLACAR]);
void salvaPlacar(TIPO_PLACAR placar[TAM_PLACAR]);
/* Funções para desenhar os botões */
void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]);
void desenhaBotaoRanking(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, TIPO_PLACAR placar);
/* Funções para desenhar os diferentes menus do jogo */
void desenhaTextoMenu(EstadoMenu opcaoMenu);
void desenhaTextoRanking(TIPO_PLACAR placar[TAM_PLACAR]);
void desenhaTextoPause(EstadoPausado opcaoPause);
void desenhaTextoVitoria(int pontos, char nomeTemp[TAM_NOME_RANKING], int *piscando, ModoVitoria modoVitoria);
void desenhaTextoDerrota(void);
/* Funções para lidar com o mapa */
void criaMapa(char mapa[TILES][TILES], int fase);
void corrigeMapa(char mapa[TILES][TILES]);
/* Funções para lidar com os pontos e placar */
void reduzPontos(int *pontos, int *framesReduzirPontos);
void reiniciaNome(char nomeTemp[TAM_NOME_RANKING]);
void colocaNoPlacar(TIPO_PLACAR placar[TAM_PLACAR], char nomeTemp[TAM_NOME_RANKING], int pontos, ModoVitoria modoVitoria);
void ordenarPlacar(TIPO_PLACAR placar[TAM_PLACAR]);

/* Funções para fazer testes que devolvem booleanos*/
int vitoria(char mapa[TILES][TILES], int fase);

#endif
