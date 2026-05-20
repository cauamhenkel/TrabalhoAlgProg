#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"

#define FPS 60
#define LARGURA 600
#define ALTURA 600
#define CABECALHO 50
#define COMP_LINHA 20
#define COMP_COLUNA 20
#define LARG_BOTOES 160
#define ALT_BOTOES 60
#define FONTE_BOTOES 20

int main(){

    /* Variáveis gerais */
    EstadoJogo estado = MENU; // Variável usada para verificar o estado do jogo, começa no menu
    int sair=0;

    /* Variáveis do menu */
    EstadoMenu opcaoMenu = MENU_JOGAR;
    /* Variáveis do pause */
    EstadoPausado opcaoPause = PAUSE_CONTINUAR;

    /* Variáveis do jogo */
    char mapa[30][30];        // Matriz do mapa
    int fase;                 // Fase no momento
    int qtdMonstros=0;        // Quantidade de monstros
    int terco=0;              // Variável para a gravidade seja contabilizada apenas a cada 3 frames
    int iniciouMapa=0, iniciouPlayer=0, iniciouMonstros=0; // Flags para iniciar apenas uma vez

    PLAYER p;                 // Variável com todas as informações do player
    MONSTRO monstros[10]={};  // Vetor com todos as informações de todos os monstros

    /* Início do código */
    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games");
    SetTargetFPS(FPS);
    HideCursor();

    while (!WindowShouldClose() && !sair) {
        switch (estado){
            case MENU:
                if (IsKeyPressed(KEY_UP)){
                    if (opcaoMenu==MENU_JOGAR)
                        opcaoMenu=MENU_SAIR;
                    else
                        opcaoMenu--;
                }
                if (IsKeyPressed(KEY_DOWN)){
                    if (opcaoMenu==MENU_SAIR)
                        opcaoMenu=MENU_JOGAR;
                    else
                        opcaoMenu++;
                }
                switch(opcaoMenu){
                    case MENU_JOGAR:
                        if (IsKeyPressed(KEY_ENTER)){
                            estado = EM_JOGO;
                            reiniciaFase(&iniciouMapa, &iniciouPlayer, &iniciouMonstros);
                            fase=0;
                            p.saude=3;
                        }
                        break;
                    case MENU_RANKING:
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                        break;
                    case MENU_SAIR:
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                }
                break;

            case PAUSADO:
                if (IsKeyPressed(KEY_UP)){
                    if (opcaoPause==PAUSE_CONTINUAR)
                        opcaoPause=PAUSE_SAIR;
                    else
                        opcaoPause--;
                }
                if (IsKeyPressed(KEY_DOWN)){
                    if (opcaoPause==PAUSE_SAIR)
                        opcaoPause=PAUSE_CONTINUAR;
                    else
                        opcaoPause++;
                }
                switch(opcaoPause){
                    case PAUSE_CONTINUAR:
                        if (IsKeyPressed(KEY_ENTER))
                            estado=EM_JOGO;
                        break;
                    case PAUSE_VOLTAR_AO_MENU:
                        if (IsKeyPressed(KEY_ENTER))
                            estado=MENU;
                        break;
                    case PAUSE_SAIR:
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                }
                break;

            case VITORIA:
                if (IsKeyPressed(KEY_ENTER))
                    estado = MENU;
                break;

            case DERROTA:
                if (IsKeyPressed(KEY_ENTER))
                    estado = MENU;
                break;

            case EM_JOGO:
                if (!iniciouMapa){
                    criaMapa(mapa, fase, &estado);
                    iniciouMapa=1;
                }
                if (!iniciouPlayer){
                    iniciaPlayer(&p, mapa);
                    iniciouPlayer=1;
                }
                if (!iniciouMonstros){
                    iniciaMonstros(monstros, mapa, &qtdMonstros);
                    iniciouMonstros=1;
                }

                regulaMovimentoMonstros(monstros, qtdMonstros, mapa);
                moveMonstros(monstros, qtdMonstros);

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
                    if (colidiuTeto(p, mapa))
                        p.velY=0;
                    gravidade(&p, &terco);
                }
                if (colidiuMonstro(p, monstros, qtdMonstros) && p.invencibilidade == 0){
                    danoPlayer(&p);
                }
                if (p.invencibilidade > 0){
                    p.invencibilidade--;
                }
                if (p.saude == 0 || caiuDoMapa(p)) {
                    estado = DERROTA;
                    fase=0;
                }
                if (playerNoFinal(p, mapa)) {
                    reiniciaFase(&iniciouMapa, &iniciouPlayer, &iniciouMonstros);
                    fase++;
                }
                if (IsKeyPressed(KEY_TAB)) {
                    estado = PAUSADO;
                    opcaoPause = PAUSE_CONTINUAR;
                }
                break;
        }

        BeginDrawing();
            switch(estado){
                case MENU:
                    ClearBackground(BLACK);
                    DrawText("Mario Games", (LARGURA/2) - (MeasureText("Mario Games", 50)/2), CABECALHO, 50, RED);
                    if (opcaoMenu==MENU_JOGAR)
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 200, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Jogar");
                    else
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 200, LARG_BOTOES, ALT_BOTOES, ORANGE, RED, "Jogar");
                    if (opcaoMenu==MENU_RANKING)
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 300, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Ranking");
                    else
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 300, LARG_BOTOES, ALT_BOTOES, ORANGE, RED, "Ranking");
                    if (opcaoMenu==MENU_SAIR)
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 400, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Sair");
                    else
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 400, LARG_BOTOES, ALT_BOTOES, ORANGE, RED, "Sair");
                    break;

                case PAUSADO:
                    ClearBackground(BLACK);
                    DrawText("Jogo pausado", (LARGURA/2) - (MeasureText("Jogo pausado", 50)/2), CABECALHO, 50, RED);
                    if (opcaoPause==PAUSE_CONTINUAR)
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 200, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Continuar");
                    else
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 200, LARG_BOTOES, ALT_BOTOES, ORANGE, RED, "Continuar");
                    if (opcaoPause==PAUSE_VOLTAR_AO_MENU)
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 300, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Voltar ao menu");
                    else
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 300, LARG_BOTOES, ALT_BOTOES, ORANGE, RED, "Voltar ao menu");
                    if (opcaoPause==PAUSE_SAIR)
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 400, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Sair");
                    else
                        desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), 400, LARG_BOTOES, ALT_BOTOES, ORANGE, RED, "Sair");
                    break;

                case VITORIA:
                    ClearBackground(BLACK);
                    DrawText("Voce venceu!", (LARGURA/2) - (MeasureText("Voce venceu!", 50)/2), CABECALHO, 50, GREEN);
                    DrawText("Pressione ENTER para voltar ao menu", (LARGURA/2) - (MeasureText("Pressione ENTER para voltar ao menu", FONTE_BOTOES)/2), (ALTURA+CABECALHO)/2, FONTE_BOTOES, WHITE);
                    break;

                case DERROTA:
                    ClearBackground(BLACK);
                    DrawText("Voce morreu!", (LARGURA/2) - (MeasureText("Voce morreu!", 50)/2), CABECALHO, 50, RED);
                    DrawText("Pressione ENTER para voltar ao menu", (LARGURA/2) - (MeasureText("Pressione ENTER para voltar ao menu", FONTE_BOTOES)/2), (ALTURA+CABECALHO)/2, FONTE_BOTOES, WHITE);
                    break;

                case EM_JOGO:
                    ClearBackground(BLACK);
                    desenhaMapa(mapa);
                    desenhaPlayer(p);
                    desenhaMonstros(monstros, qtdMonstros);
                    exibeSaude(p);
                    exibeFase(fase);
                    break;
            }
    	EndDrawing();
    }

    CloseWindow();

    return 0;
}

/* Coisas a fazer*/
// Menu de ranking
// Pontos ou tempo (na estrutura player)
// Placar (usar uma struct de placar definida pelo professor)
