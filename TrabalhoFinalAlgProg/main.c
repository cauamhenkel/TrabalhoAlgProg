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

    /* Variáveis para os pontos, vitória e ranking */
    TIPO_PLACAR placar[TAM_PLACAR]={};
    int pontos;
    int framesReduzirPontos;
    char nomeTemp[TAM_NOME_RANKING]={};
    char caractereLido;
    int posCaractereAtual;
    int piscando;

    /* Variaveis graficas */
    Spritesheet sprites;
    Vector2 ultima_morte_monstro = (Vector2){0.0f, 0.0f};
    float timer_pontos_subindo = 0.0f;

    /* Início do código */
    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games");
    SetTargetFPS(FPS);
    HideCursor();
    /* Carrega os sprites */
    carregaSpritesheet(&sprites);
    /* Carrega o placar */
    lePlacar(placar);

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
                            pontos=QTD_PONTOS_INICIAIS;
                            framesReduzirPontos=0;
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
                if (posCaractereAtual!=TAM_NOME_RANKING-1){
                    caractereLido=GetCharPressed();
                    if (caractereLido>=32 && caractereLido<=126){
                        nomeTemp[posCaractereAtual]=caractereLido;
                        posCaractereAtual++;
                    }
                }
                if (posCaractereAtual>0){
                    if (IsKeyPressed(KEY_BACKSPACE)){
                        nomeTemp[posCaractereAtual-1]='\0';
                        posCaractereAtual--;
                    }
                }
                if (IsKeyPressed(KEY_ENTER) && strlen(nomeTemp)>0){
                    colocaNoPlacar(placar, nomeTemp, pontos);
                    ordenarPlacar(placar);
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
                if (vitoria(mapa, fase)){
                    estado=VITORIA;
                    reiniciaNome(nomeTemp);
                    posCaractereAtual=0;
                    piscando=0;
                }
                else{
                    if (!iniciouFase){
                        criaMapa(mapa, fase);
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
                    reduzPontos(&pontos, &framesReduzirPontos);

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
                    processaProjetil(&p, &pr, mapa);
                    Vector2 morte_monstro = mataMonstrosProjetil(&p, &pr, monstros, qtdMonstros, &pontos);

                    if (morte_monstro.x != -1.0f) {
                        ultima_morte_monstro = morte_monstro; // Salva o ultimo ponto em que um monstro morreu
                        timer_pontos_subindo = TEMPO_PTS_SUBINDO; // Reseta o timer para exibir os pontinhos subindo
                    }
                    if (timer_pontos_subindo > 0.0f) {
                        timer_pontos_subindo -= GetFrameTime(); // Diminui o timer independentemente do FPS
                        ultima_morte_monstro.y -= 0.5f; // Faz com que os pontinhos subam 1 pixel por frame
                    }

                    if (p.naEscada && !playerNaSubida(p, mapa) &&  !playerNaEscada(p, mapa) && !playerNaDescida(p, mapa) && !playerNaEscadaComPlataforma(p, mapa)){
                        // Caso o player ocorra de subir demais a escada
                        corrigePersonagemY(&p); // Corrige o personagem (acaba jogando ele pra cima)
                        p.posY+=COMP_COLUNA;    // Bota ele pra baixo
                    }

                    if (colidiuMonstro(p, monstros, qtdMonstros) && p.invencibilidade == 0){
                        danoPlayer(&p);
                        pontos-=QTD_REDUZIR_PONTOS*5;
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
                }
                break;
        }

        BeginDrawing();
            switch(estado){
                case MENU:
                    ClearBackground(BLACK);
                    desenhaTextoMenu(opcaoMenu);
                    break;

                case RANKING:
                    ClearBackground(BLACK);
                    desenhaTextoRanking(placar);
                    break;

                case PAUSADO:
                    ClearBackground(BLACK);
                    desenhaTextoPause(opcaoPause);
                    break;

                case VITORIA:
                    ClearBackground(BLACK);
                    desenhaTextoVitoria(pontos, nomeTemp, &piscando);
                    break;

                case DERROTA:
                    ClearBackground(BLACK);
                    desenhaTextoDerrota();
                    break;

                case EM_JOGO:
                    ClearBackground(GRAY);
                    exibeCabecalho(p, fase, pontos);
                    desenhaMapa(mapa, sprites.tileset);

                    atualizaAnimacaoPlayer(&p);
                    desenhaPlayer(p, sprites.player);

                    atualizaAnimacaoMonstros(monstros, qtdMonstros);
                    desenhaMonstros(monstros, qtdMonstros, sprites.monstro);

                    desenhaProjetil(pr);
                    if (timer_pontos_subindo > 0.0f)
                        desenhaPontosSubindo(ultima_morte_monstro);
                    break;
            }
    	EndDrawing();
    }
    salvaPlacar(placar);
    descarregaSpritesheet(&sprites);
    CloseWindow();
    return 0;
}

/* Coisas a fazer*/
