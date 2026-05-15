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
#define FPS 60

int main(){

    char mapa[30][30];        // Matriz do mapa
    char nomeMapa[12];        // String do nome do mapa que servirá de base para abrir o arquivo
    int qtdMonstros=0;        // Quantidade de monstros
    int terco=0;              // Variável para a gravidade seja contabilizada apenas a cada 3 frames
    int morreu=0, venceu=0;   // Flags para encerrar o jogo
    int iniciouMapa=0, iniciouPlayer=0, iniciouMonstros=0; // Flags para iniciar apenas uma vez

    PLAYER p;                 // Variável com todas as informações do player
    MONSTRO monstros[10]={};  // Vetor com todos as informações de todos os monstros
    EstadoJogo estado = MENU; // Variável usada para verificar o estado do jogo, começa no menu

    InitWindow(LARGURA, ALTURA+CABECALHO, "Mario Games");
    SetTargetFPS(FPS);
    HideCursor();

    while (!WindowShouldClose()) {
        switch (estado){                       // Verifica qual o estado do jogo atualmente
            case MENU:                         // Se for o menu
            if (IsKeyPressed(KEY_ENTER))       // Inicia o jogo com enter
                estado = EM_JOGO;
            break;

            case EM_JOGO:                      // Se for o jogo
            if (!iniciouMapa){                 // Inicia o mapa apenas uma vez
                arqMapa(nomeMapa, 0);
                criaMapa(mapa, nomeMapa);
                iniciouMapa=1;
            }
            if (!iniciouPlayer){               // Inicia o player apenas uma vez
                iniciaPlayer(&p, mapa);
                iniciouPlayer=1;
            }
            if (!iniciouMonstros){             // Inicia os monstros apenas uma vez
                iniciaMonstros(monstros, mapa, &qtdMonstros);
                iniciouMonstros=1;
            }

            regulaMovimentoMonstros(monstros, qtdMonstros, mapa); // Avalia as posições e velocidades dos monstros e altera a velocidade se necessário
        	moveMonstros(monstros, qtdMonstros);                  // Muda a posição dos monstros

            if (IsKeyDown(KEY_D)){             // Testa o movimento para a direita
                if (!colidiuBordaDireita(p)){  // Muda a posição enquanto não bater na borda
                    p.posX+=p.velX;
                }
            }
            if (IsKeyDown(KEY_A)){             // Testa o movimento para a esquerda
                if (!colidiuBordaEsquerda(p)){ // Muda a posição enquanto não bater na borda
                    p.posX-=p.velX;
                }
            }
            if (IsKeyPressed(KEY_W)){          // Testa se o player quer tentar subir a escada
                if (playerNaEscada(p, mapa)){  // Sobe a escada se estiver na posição correta
                    sobeEscada(&p, mapa);
                }
            }
            if (IsKeyPressed(KEY_S)){          // Testa se o player quer tentar descer a escada
                if (playerNaDescida(p, mapa)){ // Desce a escada se estiver na posição correta
                    desceEscada(&p, mapa);
                }
        	}
        	if (IsKeyPressed(KEY_SPACE)){      // Testa se o player quer pular
                if (playerNoChao(p, mapa)){    // Testa se o player está no chão (impede pulo no ar)
                    p.posY-=COMP_COLUNA/4;     // Muda a posição do player, tira ele do chão
                    p.velY-=COMP_COLUNA/2;     // Muda sua velocidade vertical
                }
        	}
        	if (playerNoChao(p, mapa)){        // Se o player está no chão
                corrigePersonagem(&p);         // Corrige posicionamento em relação ao chão, impede de entrar nele
                p.velY=0;                      // Mantém a posição vertical estática
                p.accY=0;
        	}
        	else{                              // Se não está no chão
                gravidade(&p, &terco);         // Aplica gravidade
        	}
            if (colidiuMonstro(p, monstros, qtdMonstros) && p.invencibilidade == 0){      // Se o player colide com o monstro e não está momentaneamente invencível
                danoPlayer(&p);                                                           // Causa dano no player (diminui sua vida em 1 ponto)
            }
            if (p.invencibilidade > 0){        // Se o player ainda possui invencibilidade
                p.invencibilidade--;           // Diminui 1 ponto por frame
            }
            if (p.saude == 0 || caiuDoMapa(p)) {                                          // Se o player morreu ou caiu do mapa
                matarPlayer(&p, &estado, &iniciouMapa, &iniciouPlayer, &iniciouMonstros); // Mata o player
                morreu=1;                                                                 // Ativa flag de morte
            }
            if (playerNoFinal(p, mapa)) {                                                 // Se o player chegou no final
                matarPlayer(&p, &estado, &iniciouMapa, &iniciouPlayer, &iniciouMonstros); // Reinicia o player (rever o uso dessa função)
                venceu=1;                                                                 // Ativa flag de vitória
            }
            if (IsKeyPressed(KEY_TAB)) {       // Se apertar TAB
                estado = PAUSADO;              // Pausa o jogo
            }
            break;

            case PAUSADO:                      // Se o jogo estiver pausado
            if (IsKeyPressed(KEY_TAB))         // Se apertar TAB
                estado = EM_JOGO;              // Despausa o jogo
            break;

        }

        BeginDrawing();
            if (estado == MENU) {              // Se está no menu
                ClearBackground(BLACK);        // Desenha o menu (rever o uso de menu)
                if (morreu) {                  // Se é menu de morte, desenha o menu de morte
                    DrawText("Voce morreu!", 25, (ALTURA+CABECALHO)/2-35, 35, RED);
                    DrawText("Pressione ENTER para recomecar", 25, (ALTURA+CABECALHO)/2, 25, WHITE);
                }
                else if (venceu) {             // Se é o menu de vitória, desenha o menu de vitória
                    DrawText("Voce venceu!", 25, (ALTURA+CABECALHO)/2-35, 35, GREEN);
                    DrawText("Pressione ENTER para recomecar", 25, (ALTURA+CABECALHO)/2, 25, WHITE);
                }
                else {                         // Se não é nenhum dos dois, desenha o menu inicial
                    DrawText("Pressione ENTER para comecar", 25, (ALTURA+CABECALHO)/2, 25, WHITE);
                }
            }
            else {                             // Se não for o menu
                ClearBackground(BLACK);        // Desenha o jogo
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

/* Coisas a fazer*/
// Menu principal
// Mudar o menu de pause
// Indicação da fase
// Fazer um jeito de ir para a proxima fase (talvez incluir algum sistema onde tenta ir lendo o arquivo de 0 a infinito (incrementando)
// e dar a vitória quando não conseguir mais ler nada, ou seja, leu todos os arquivos mapaX.txt)
// Alguma coisa para impedir de atravessar o teto pulando
// Pontos ou tempo (na estrutura player)
// Placar (usar uma struct de placar definida pelo professor)
