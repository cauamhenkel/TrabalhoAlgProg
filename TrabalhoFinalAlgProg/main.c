#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"

#define LARGURA 600
#define ALTURA 600
#define CABECALHO 50
#define COMP_LINHA 20
#define COMP_COLUNA 20

int main(){

    char mapa[30][30];
    char nomeMapa[12];
    int qtdMonstros;
    int terco=0;
    PLAYER p;
    MONSTRO monstros[10]={};

    arqMapa(nomeMapa,"2");
    criaMapa(mapa,nomeMapa);

    qtdMonstros = calculaQtdMonstros(mapa);

    iniciaPlayer(&p, mapa);
    iniciaMonstros(monstros, mapa);

    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games DOIS");
    SetTargetFPS(60);
    HideCursor();

    while (!WindowShouldClose())
    {

    if (IsKeyDown(KEY_D)){
        if (!colidiuBordaDireita(p)){
            p.posX+=p.velX;
        }
    }
    if (IsKeyDown(KEY_A)){
        if (!colidiuBordaEsquerda(p)){
            p.posX-=p.velX;
        }
    }
    if (IsKeyPressed(KEY_W)){
        if (playerNaEscada(p, mapa)){
            sobeEscada(&p, mapa);
        }
    }
    if (IsKeyPressed(KEY_S)){
        if (playerNaDescida(p, mapa)){
            desceEscada(&p, mapa);
        }
	}
	if (IsKeyPressed(KEY_SPACE)){
        p.posY-=COMP_COLUNA/4;
        p.velY-=COMP_COLUNA/2;
	}
	if (playerNoChao(p, mapa)){
        corrigePersonagem(&p);
        p.velY=0;
        p.accY=0;
	}
	else{
        gravidade(&p, &terco);
	}

	moveMonstros(monstros, qtdMonstros);

    BeginDrawing();
	ClearBackground(RAYWHITE);

	desenhaMapa(mapa);

    desenhaPlayer(p);
    desenhaMonstros(monstros, qtdMonstros);

	EndDrawing();
    }

    CloseWindow();

    return 0;
}
