#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "raylib.h"
#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"
#include "graficos.h"
#include "extras.h"

int main(){

    /* Variáveis gerais */
    EstadoJogo estado = MENU; // Variável usada para verificar o estado do jogo, começa no menu
    int sair=0;               // Flag usada para fechar o jogo

    /* Variaveis para padronizar a posicao dos botoes em menus: */
    int posBotao1 = COMP_COLUNA * 10,
        posBotao2 = posBotao1 + (COMP_COLUNA * 5),
        posBotao3 = posBotao2 + (COMP_COLUNA * 5);

    /* Variáveis do menu */
    EstadoMenu opcaoMenu = MENU_JOGAR;

    /* Variáveis do pause */
    EstadoPausado opcaoPause = PAUSE_CONTINUAR;

    /* Variáveis do jogo */
    char mapa[TILES][TILES];         // Matriz do mapa
    int fase;                        // Fase no momento
    int qtdMonstros=0;               // Quantidade de monstros
    int iniciouFase=0;               // Flag para iniciar apenas uma vez
    PLAYER p;                        // Variável com todas as informações do player
    MONSTRO monstros[M_QTD_MAX]={};  // Vetor com todos as informações de todos os monstros
    PROJETIL pr;

    /* Variáveis para os pontos */
    int pontos;
    TIPO_PLACAR placar[10];
    float tempoAtual, tempoAnterior;

    /* Início do código */
    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games");
    SetTargetFPS(FPS);
    HideCursor();
    /* SPRITES */
    Spritesheet sprites;
    carregaSpritesheet(&sprites);

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
                            // Coisas que precisam ser iniciadas todo inicio de jogo (e não inicio de fase)
                            estado = EM_JOGO;
                            iniciouFase=0;
                            fase=0;
                            p.saude=P_VIDA_MAX;
                            pontos=5000;
                            tempoAnterior=0;
                        }
                        break;
                    case MENU_RANKING:
                        if (IsKeyPressed(KEY_ENTER))
                            estado = RANKING;

                        break;
                    case MENU_SAIR:
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                }
                break;

                case RANKING:
                    if (IsKeyPressed(KEY_ENTER)){
                        estado=MENU;
                        opcaoMenu=MENU_JOGAR;
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
                            opcaoMenu=MENU_JOGAR;
                        break;
                    case PAUSE_SAIR:
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                }
                break;

            case VITORIA:
                if (IsKeyPressed(KEY_ENTER)){
                    estado = MENU;
                    opcaoMenu=MENU_JOGAR;
                }
                break;

            case DERROTA:
                if (IsKeyPressed(KEY_ENTER)){
                    estado = MENU;
                    opcaoMenu=MENU_JOGAR;
                }
                break;

            case EM_JOGO:
                if (!iniciouFase){
                    criaMapa(mapa, fase, &estado);
                    corrigeMapa(mapa);
                    iniciaPlayer(&p, mapa);
                    iniciaMonstros(monstros, mapa, &qtdMonstros);
                    pr.estado=DESATIVADO;
                    iniciouFase=1;
                }

                if (IsKeyPressed(KEY_TAB)) {
                    estado = PAUSADO;
                    opcaoPause = PAUSE_CONTINUAR;
                }

                /* Pontos */
                reduzPontos(&pontos, &tempoAtual, &tempoAnterior);

                /* MOVIMENTACAO MONSTROS */
                regulaMovimentoMonstros(monstros, qtdMonstros, mapa);
                moveMonstros(monstros, qtdMonstros);

                /* MOVIMENTACAO PLAYER */
                processaMovimentoPlayer(&p, mapa);
                processaPlayerNaEscada(&p, mapa);
                processaPuloPlayer(&p, mapa);

                /* FISICA PLAYER */
                controlaGravidadePlayer(&p, mapa);
                processaGravidadePlayer(&p);
                processaAceleracaoPlayer(&p);
                processaAtritoPlayer(&p);

                /* COLISOES PLAYER */
                processaColisoesPlayer(&p, mapa);

                /* PROJETIL */
                if (IsKeyPressed(KEY_K) && !p.naEscada && p.qtdTiros>0 && p.cooldownTiro==0){
                    criaProjetil(p, &pr);
                    p.cooldownTiro=P_COOLDOWN_TIRO;

                    if (p.qtdTiros>0){
                        p.qtdTiros--;
                    }
                }
                if (pr.estado==ATIVO){
                    moveProjetil(&pr);
                    processaColisoesProjetil(&pr, monstros, mapa);
                }
                if (p.cooldownTiro>0){
                    p.cooldownTiro--;
                }
                mataMonstrosProjetil(&p, &pr, monstros, qtdMonstros, &pontos);

                if (p.naEscada && !playerNaSubida(p, mapa) &&  !playerNaEscada(p, mapa) && !playerNaDescida(p, mapa) && !playerNaEscadaComPlataforma(p, mapa)){
                    // Caso o player ocorra de subir demais a escada
                    corrigePersonagemY(&p); // Corrige o personagem (acaba jogando ele pra cima)
                    p.posY+=COMP_COLUNA;    // Bota ele pra baixo
                }

                if (colidiuMonstro(p, monstros, qtdMonstros) && p.invencibilidade == 0){
                    danoPlayer(&p);
                    pontos-=1000;
                }
                if (p.invencibilidade > 0){
                    p.invencibilidade--;
                }
                if (p.saude == 0 || caiuDoMapa(p)) {
                    estado = DERROTA;
                }
                if (playerNoFinal(p, mapa)) {
                    iniciouFase=0;
                    fase++;
                }
                break;
        }

        BeginDrawing();
            switch(estado){
                case MENU:
                    ClearBackground(BLACK);
                    DrawText("Mario Games", (LARGURA/2) - (MeasureText("Mario Games", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao1, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_JOGAR) ? GOLD : ORANGE, RED, "Jogar");
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao2, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_RANKING) ? GOLD : ORANGE, RED, "Ranking");
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao3, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_SAIR) ? GOLD : ORANGE, RED, "Sair");
                    break;

                case RANKING:
                    ClearBackground(BLACK);
                    DrawText("Ranking", (LARGURA/2) - (MeasureText("Ranking", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), COMP_COLUNA * 25, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Voltar ao menu");
                    break;

                case PAUSADO:
                    ClearBackground(BLACK);
                    DrawText("Jogo pausado", (LARGURA/2) - (MeasureText("Jogo pausado", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao1, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_CONTINUAR) ? GOLD : ORANGE, RED, "Continuar");
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao2, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_VOLTAR_AO_MENU) ? GOLD : ORANGE, RED, "Voltar ao menu");
                    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao3, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_SAIR) ? GOLD : ORANGE, RED, "Sair");
                    break;

                case VITORIA:
                    ClearBackground(BLACK);
                    DrawText("Voce venceu!", (LARGURA/2) - (MeasureText("Voce venceu!", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, GREEN);
                    DrawText("Pressione ENTER para voltar ao menu", (LARGURA/2) - (MeasureText("Pressione ENTER para voltar ao menu", FONTE_CABECALHO)/2), (ALTURA+CABECALHO)/2, FONTE_CABECALHO, WHITE);
                    break;

                case DERROTA:
                    ClearBackground(BLACK);
                    DrawText("Voce morreu!", (LARGURA/2) - (MeasureText("Voce morreu!", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
                    DrawText("Pressione ENTER para voltar ao menu", (LARGURA/2) - (MeasureText("Pressione ENTER para voltar ao menu", FONTE_CABECALHO)/2), (ALTURA+CABECALHO)/2, FONTE_CABECALHO, WHITE);
                    break;

                case EM_JOGO:
                    ClearBackground(GRAY);
                    desenhaMapa(mapa, sprites.tileset);
                    desenhaPlayer(p, sprites.player);
                    desenhaMonstros(monstros, qtdMonstros, sprites.monstro);
                    desenhaProjetil(pr);
                    exibeCabecalho(p, fase, pontos);
                    break;
            }
    	EndDrawing();
    }

    descarregaSpritesheet(&sprites);
    CloseWindow();
    return 0;
}

/* Coisas a fazer*/
// Menu de ranking
// Placar (usar uma struct de placar definida pelo professor)
