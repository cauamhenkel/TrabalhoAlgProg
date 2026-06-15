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
    EstadoJogo estado = MENU;                             // Variável usada para verificar o estado do jogo, começa no menu
    int sair=0;                                           // Flag usada para fechar o jogo

    /* Variáveis do menu */
    EstadoMenu opcaoMenu = MENU_JOGAR;                    // Variável para verificar qual opção do menu o jogador selecionou

    /* Variáveis do pause */
    EstadoPausado opcaoPause = PAUSE_CONTINUAR;           // Variável para verificar qual opção do menu de pause o jogador selecionou

    /* Variáveis do jogo */
    char mapa[TILES][TILES];                              // Matriz do mapa
    int fase;                                             // Fase no momento
    int qtdMonstros=0;                                    // Quantidade de monstros
    int qtdMonstrosMortos=0, qtdTotalMonstros=0;          // Acumuladores para implementar a recompensa do modo pacifista
    int iniciouFase=0;                                    // Flag para iniciar apenas uma vez
    PLAYER p;                                             // Variável com todas as informações do player
    MONSTRO monstros[M_QTD_MAX]={};                       // Vetor com todos as informações de todos os monstros
    PROJETIL pr;

    /* Variáveis para os pontos, vitória e ranking */
    TIPO_PLACAR placar[TAM_PLACAR]={};                    // Vetor contendo todo o placar
    int pontos;                                           // Variável contendo os pontos do jogo atual
    int framesReduzirPontos;                              // Variável que conta os frames para reduzir os pontos
    char nomeTemp[TAM_NOME_RANKING+1]={};                 // Nome temporário que será escrito no menu de ranking e depois será salvo no placar
    char caractereLido;                                   // Último caractere digitado pelo jogador
    int posCaractereAtual;                                // Posição atual na string do nome
    int piscando;                                         // Variável para a barra no menu de vitória ficar piscando
    ModoVitoria modoVitoria;                              // Variável para avaliar o modo em que o player venceu

    /* Variáveis gráficas */
    Spritesheet sprites;                                  // Varoável para salvar os sprites da memória
    Vector2 ultima_morte_monstro = (Vector2){0.0f, 0.0f}; // Variável para salvar a posição do monstro que foi morto
    float timer_pontos_subindo = 0.0f;                    // Variável para o tempo que os pontos ficam subindo

    /* Variáveis de som */
    Soundtrack sounds;                                    // Variável para salvar os sons da memória

    /* Início do código */
    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games");
    SetTargetFPS(FPS);
    HideCursor();

    /* SPRITES */
    carregaSpritesheet(&sprites);

    /* SOUNDTRACK */
    InitAudioDevice();
    carregaSoundtrack(&sounds);

    /* Carrega o placar */
    lePlacar(placar);

    // Mantém o loop enquanto o jogador não resolver sair
    while (!WindowShouldClose() && !sair){
        // Verifica o estado de jogo atual e roda o código separado de cada parte
        switch (estado){
            case MENU:
                // Roda o código do menu
                // Muda a opção selecionada com as setinhas
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
                        // Se pressionar enter quando estiver nessa opção inicia o jogo
                        if (IsKeyPressed(KEY_ENTER)){
                            PlaySound(sounds.botao);

                            // Coisas que precisam ser iniciadas todo inicio de jogo (e não inicio de fase)
                            estado=EM_JOGO;
                            iniciouFase=0;
                            // Inicia na fase 0
                            fase=0;
                            // Acumulador de mortes para verificar se o player terminou sem matar nenhum monstro
                            qtdMonstrosMortos=0;
                            qtdTotalMonstros=0;
                            // Inicia vida e os tiros do player
                            p.saude=P_VIDA_MAX;
                            p.qtdTiros=P_QTD_TIROS;
                            // Inicia os pontos e o contador para reduzí-los, o modo padrão de vitória é 'NORMAL'
                            pontos=QTD_PONTOS_INICIAIS;
                            framesReduzirPontos=0;
                            modoVitoria=NORMAL;
                        }
                        break;
                    case MENU_RANKING:
                        // Se pressionar enter quando estiver nessa opção entra no ranking
                        if (IsKeyPressed(KEY_ENTER)){
                            PlaySound(sounds.botao);
                            estado = RANKING;
                        }

                        break;
                    case MENU_SAIR:
                        // Se pressionar enter quando estiver nessa opção fecha o jogo
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                }
                break;

                case RANKING:
                    // Roda o código do ranking
                    // Se estiver no ranking a única interação possível é sair
                    if (IsKeyPressed(KEY_ENTER)){
                        PlaySound(sounds.botao);
                        estado=MENU;
                        opcaoMenu=MENU_JOGAR;
                    }
                    break;

            case PAUSADO:
                // Roda o código do menu de pause
                // Muda a opção selecionada com as setinhas
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
                        // Se pressionar enter quando estiver nessa opção volta para o jogo
                        if (IsKeyPressed(KEY_ENTER)){
                            PlaySound(sounds.botao);
                            estado=EM_JOGO;
                        }
                        break;
                    case PAUSE_VOLTAR_AO_MENU:
                        // Se pressionar enter quando estiver nessa opção volta para o menu
                        if (IsKeyPressed(KEY_ENTER)){
                            PlaySound(sounds.botao);
                            estado=MENU;
                            opcaoMenu=MENU_JOGAR;
                        }
                        break;
                    case PAUSE_SAIR:
                        // Se pressionar enter quando estiver nessa opção fecha o jogo
                        if (IsKeyPressed(KEY_ENTER))
                            sair=1;
                }
                break;

            case VITORIA:
                // Se não chegou no fim da string, pega o caractere lido (se for pressionado algo) e coloca na string
                if (posCaractereAtual!=TAM_NOME_RANKING-1){
                    caractereLido=GetCharPressed();
                    if (caractereLido>=32 && caractereLido<=126){
                        nomeTemp[posCaractereAtual]=caractereLido;
                        posCaractereAtual++;
                    }
                }
                // Se tive pelo menus um caractere permite apagar o último caractere digitado
                if (posCaractereAtual>0){
                    if (IsKeyPressed(KEY_BACKSPACE)){
                        nomeTemp[posCaractereAtual-1]='\0';
                        posCaractereAtual--;
                    }
                }
                // Se tiver pelo menos um único caractere permite salvar o nome no placar
                if (IsKeyPressed(KEY_ENTER) && strlen(nomeTemp)>0){
                    PlaySound(sounds.botao);
                    // Coloca o nome e pontos no placar, ordena ele e volta para o menu
                    colocaNoPlacar(placar, nomeTemp, pontos, modoVitoria);
                    ordenarPlacar(placar);
                    estado = MENU;
                    opcaoMenu=MENU_JOGAR;
                }
                break;

            case DERROTA:
                // Roda o código do menu de pause
                // Se estiver no ranking a única interação possível é voltar para o menu
                if (IsKeyPressed(KEY_ENTER)){
                    PlaySound(sounds.botao);
                    estado = MENU;
                    opcaoMenu=MENU_JOGAR;
                }
                break;

            case EM_JOGO:
                // Roda o código do jogo
                // Testa a vitória
                if (vitoria(mapa, fase)){
                   // Coisas que precisam ser iniciadas toda vez que se vence o jogo
                    estado=VITORIA;
                    reiniciaNome(nomeTemp);
                    posCaractereAtual=0;
                    piscando=0;
                    if (qtdMonstrosMortos==0){
                        modoVitoria=PACIFISTA;
                        pontos+=(qtdTotalMonstros*QTD_PONTOS_GANHO_DOS_INIMIGOS*1.2);
                    }
                    if (qtdMonstrosMortos==qtdTotalMonstros){
                        modoVitoria=GENOCIDA;
                        pontos+=(qtdTotalMonstros*QTD_PONTOS_GANHO_DOS_INIMIGOS*0.1);
                    }
                }
                // Se não venceu
                else{
                    // Roda esse código apenas uma vez no inicio da fase
                    if (!iniciouFase){
                        // Cria o grid do mapa
                        criaMapa(mapa, fase);
                        corrigeMapa(mapa);
                        // Inicia as estruturas
                        iniciaPlayer(&p, mapa);
                        iniciaMonstros(monstros, mapa, &qtdMonstros);
                        pr.estado=DESATIVADO;
                        // Acumula a quantidade total de monstros que havia nos mapas
                        qtdTotalMonstros+=qtdMonstros;
                        iniciouFase=1;
                    }

                    // Permite trocar para o menu de pause
                    if (IsKeyPressed(KEY_TAB)) {
                        estado = PAUSADO;
                        opcaoPause = PAUSE_CONTINUAR;
                    }

                    // Reduz os pontos depois que se passa um tempo determinado
                    reduzPontos(&pontos, &framesReduzirPontos);

                    /* MOVIMENTACAO MONSTROS */
                    regulaMovimentoMonstros(monstros, qtdMonstros, mapa);
                    moveMonstros(monstros, qtdMonstros);

                    /* MOVIMENTACAO PLAYER */
                    processaMovimentoPlayer(&p, mapa);
                    processaPlayerNaEscada(&p, mapa);
                    corrigePersonagemEscada(&p, mapa);
                    processaPuloPlayer(&p, mapa, sounds.salto);

                    /* FISICA PLAYER */
                    controlaGravidadePlayer(&p, mapa);
                    processaGravidadePlayer(&p);
                    processaAceleracaoPlayer(&p);
                    processaAtritoPlayer(&p);

                    /* COLISOES PLAYER */
                    processaColisoesAmbientePlayer(&p, mapa);
                    processaColisoesMonstros(&p, monstros, qtdMonstros, &pontos);

                    /* PROJETIL */
                    processaProjetil(&p, &pr, mapa, sounds);
                    Vector2 morte_monstro = mataMonstrosProjetil(&p, &pr, monstros, qtdMonstros, &qtdMonstrosMortos, &pontos);
                    if (morte_monstro.x != -1.0f) {
                        PlaySound(sounds.monstro_dano);
                        ultima_morte_monstro = morte_monstro; // Salva o ultimo ponto em que um monstro morreu
                        timer_pontos_subindo = TEMPO_PTS_SUBINDO; // Reseta o timer para exibir os pontinhos subindo
                    }
                    if (timer_pontos_subindo > 0.0f) {
                        timer_pontos_subindo -= GetFrameTime(); // Diminui o timer independentemente do FPS
                        ultima_morte_monstro.y -= 0.5f; // Faz com que os pontinhos subam 1 pixel por frame
                    }

                    // Se o player morrer ou cair do mapa acaba o jogo e garante a derrota
                    if (p.saude == 0 || caiuDoMapa(p)) {
                        estado = DERROTA;
                    }
                    // Se o player chegou no cristal vai para a próxima fase (ou garante a vitória)
                    if (playerNoFinal(p, mapa)) {
                        iniciouFase=0;
                        fase++;
                    }
                }
                break;
        }
        // Funções para desenhar as coisas na tela
        BeginDrawing();
            switch(estado){
                case MENU:
                    // Desenha o menu
                    ClearBackground(DARKGRAY);
                    desenhaBackground(sprites);
                    desenhaTextoMenu(opcaoMenu);
                    break;

                case RANKING:
                    // Desenha o menu de ranking
                    ClearBackground(DARKGRAY);
                    desenhaBackground(sprites);
                    desenhaTextoRanking(placar);
                    break;

                case PAUSADO:
                    // Desenha o menu de pause
                    ClearBackground(DARKGRAY);
                    desenhaBackground(sprites);
                    desenhaTextoPause(opcaoPause);
                    break;

                case VITORIA:
                    // Desenha o menu de vitória
                    ClearBackground(DARKGRAY);
                    desenhaBackground(sprites);
                    desenhaTextoVitoria(pontos, nomeTemp, &piscando, modoVitoria);
                    break;

                case DERROTA:
                    // Desenha o menu de derrota
                    ClearBackground(DARKGRAY);
                    desenhaBackground(sprites);
                    desenhaTextoDerrota();
                    break;

                case EM_JOGO:
                    // Desenha o jogo
                    ClearBackground(GRAY);
                    desenhaBackground(sprites);

                    exibeCabecalho(p, sprites, fase, pontos);
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
    // Salva os dados necessários na memória e fecha o que precisa ser fechado
    salvaPlacar(placar);
    descarregaSpritesheet(&sprites);
    descarregaSoundtrack(&sounds);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
