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
    int morreu=0, venceu=0;
    PLAYER p;
    MONSTRO monstros[10]={};
    EstadoJogo estado = MENU;

    arqMapa(nomeMapa, "0");
    criaMapa(mapa, nomeMapa);

    qtdMonstros = calculaQtdMonstros(mapa);

    iniciaPlayer(&p, mapa);
    iniciaMonstros(monstros, mapa);

    // Guardar a posicao inicial do player pra usar mais tarde
    int posX0 = p.posX;
    int posY0 = p.posY;

    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games");
    SetTargetFPS(60);
    HideCursor();

    while (!WindowShouldClose()) {
        switch (estado) {
            case MENU:
            if (IsKeyPressed(KEY_ENTER))
                estado = EM_JOGO;
            break;

            case EM_JOGO:
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
                if (playerNoChao(p, mapa)){
                    p.posY-=COMP_COLUNA/4;
                    p.velY-=COMP_COLUNA/2;
                }
        	}
        	if (playerNoChao(p, mapa)){
                corrigePersonagem(&p);
                p.velY=0;
                p.accY=0;
        	}
        	else{
                gravidade(&p, &terco);
        	}

        	regulaMovimentoMonstros(monstros, qtdMonstros, mapa);
        	moveMonstros(monstros, qtdMonstros);

            if (colidiuMonstro(p, monstros, qtdMonstros) && p.invencibilidade == 0){
                danoPlayer(&p);
            }
            if (p.invencibilidade > 0) p.invencibilidade--;

            if (p.saude == 0 || caiuDoMapa(p)) {
                matarPlayer(&p, &estado, posX0, posY0);
                morreu=1;
                venceu=0;
            }

            if (playerNoFinal(p, mapa)) {
                matarPlayer(&p, &estado, posX0, posY0);
                venceu=1;
                morreu=0;
            }

            if (IsKeyPressed(KEY_P)) {
                estado = PAUSADO;
            }
            break;

            case PAUSADO:
            if (IsKeyPressed(KEY_P))
                estado = EM_JOGO;
            break;

            }

        BeginDrawing();
            if (estado == MENU) {
                ClearBackground(BLACK);
                if (morreu) {
                    DrawText("Voce foi brutalmente estuprado", 25, (ALTURA+CABECALHO)/2-35, 35, RED);
                    DrawText("Pressione ENTER para recomecar", 25, (ALTURA+CABECALHO)/2, 25, WHITE);
                }
                else if (venceu) {
                    DrawText("Voce venceu!", 25, (ALTURA+CABECALHO)/2-35, 35, GREEN);
                    DrawText("Pressione ENTER para recomecar", 25, (ALTURA+CABECALHO)/2, 25, WHITE);
                }
                else {
                    DrawText("Pressione ENTER para comecar", 25, (ALTURA+CABECALHO)/2, 25, WHITE);
                }
            }
            else {
                ClearBackground(BLACK);
               	desenhaMapa(mapa);
                desenhaPlayer(p);
                desenhaMonstros(monstros, qtdMonstros);

                exibeSaude(p);
            }
    	EndDrawing();
        }

    CloseWindow();

    return 0;
}
