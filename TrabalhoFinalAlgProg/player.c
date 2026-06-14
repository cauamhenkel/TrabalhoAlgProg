#include "player.h"
#include "graficos.h"
#include "raylib.h"

void iniciaPlayer(PLAYER *p, char mapa[TILES][TILES]){
/* Essa função inicia os valores do player no inicio da fase */
    // Procura o player no mapa
    for (int i=0 ; i<TILES ; i++){
        for (int j=0 ; j<TILES ; j++){
            // Quando encontra atualiza sua posição
            if (mapa[i][j]=='P'){
                (*p).posX=j*COMP_LINHA;
                (*p).posY=i*COMP_COLUNA;
            }
        }
    }
    // Inicia alguns valores básicos
    p->dir=DIREITA;
    p->velX=0;
    p->velY=0;
    p->accX=P_ACC_X;
    p->invencibilidade=0;
    p->naEscada=0;
    // p->qtdTiros=P_QTD_TIROS;
    p->cooldownTiro=0;
    p->animacaoTimer=0;
    p->frameEscada=0;
    p->frameCaminhada=0;
}

void controlaGravidadePlayer(PLAYER *p, char mapa[TILES][TILES]){
/* Função para atualizar uma flag na estrutura player */
    // Desativa a gravidade se estiver em uma escada ou em cima de um chão ou plataforma
    if (p->naEscada || playerNoChao(p, mapa) || playerNaPlataforma(p, mapa)) {
        p->afetadoGravidade = 0;
    }
    else
        p->afetadoGravidade = 1;
}

void processaGravidadePlayer(PLAYER *p){
/* Função que aplica a gravidade no player */
    if (p->afetadoGravidade) {
        // Atualiza a aceleração vertical do player
        p->accY = GRAVIDADE;
        // Impede que sua velocidade vertical aumente infinitamente
        if (p->velY<P_VEL_Y_MAX){
            p->velY+=p->accY;
        }
    }
}

void processaAceleracaoPlayer(PLAYER *p){
/* Essa função atualiza a posição do player baseado nas suas velocidades */
    p->posX += p->velX;
    p->posY += p->velY;
}

void processaAtritoPlayer(PLAYER *p){
/* Essa função aplica o atrito sobre a velocidade do player */
    // Aplica uma força contrária para fazer com que sua velocidade sempre chegue a zero caso pare de clicar nos botões para se mover
    if (p->velX > 0) {
        p->velX -= FORCA_ATRITO;
    }
    if (p->velX < 0) {
        p->velX += FORCA_ATRITO;
    }
}

void corrigePersonagemY(PLAYER *p){
    // Corrige a posição vertical do personagem caso ele acabe entrando no chão
    p->posY-=(p->posY % COMP_COLUNA);
}

void centralizaPlayerNaEscada(PLAYER *p, char mapa[TILES][TILES]){
/* Essa função arruma a posição do player quando ele sobe a escada */
    // Transforma a posição do player em grid
    int posXGrid=p->posX/COMP_LINHA;
    int posYGrid=p->posY/COMP_COLUNA;
    // Se a posição atual do grid ou a da direita for de algum dos tipos referente a escada, corrige sua posição e reinicia sua velocidade
    if (mapa[posYGrid][posXGrid]=='S' || mapa[posYGrid][posXGrid]=='H' || mapa[posYGrid][posXGrid]=='D' || mapa[posYGrid][posXGrid]=='X'){
        p->velX=0;
        p->velY=0;
        p->posX-=(p->posX % COMP_LINHA);
    }
    else if (mapa[posYGrid][posXGrid+1]=='S' || mapa[posYGrid][posXGrid+1]=='H' || mapa[posYGrid][posXGrid+1]=='D' || mapa[posYGrid][posXGrid+1]=='X'){
        p->velX=0;
        p->velY=0;
        p->posX-=(p->posX % COMP_LINHA);
        p->posX+=COMP_LINHA;
    }
}

void danoPlayer(PLAYER *p){
/* Essa função aplica os efeitos no player de tomar dano */
    // Diminui a vida do player e dá um tempo de invencibilidade
    p->saude--;
    p->invencibilidade = FPS;
}

void processaMovimentoPlayer(PLAYER *p, char mapa[TILES][TILES]){
/* Essa é uma função geral que processa os movimentos do player */
    // Se o player clicar o botão para andar para a direita
    if (IsKeyDown(KEY_D)){
        // Tira ele da escada e muda sua direção
        p->naEscada = 0;
        p->dir=DIREITA;
        // Se não está na sua velocidade máxima horizontal aumenta ela com base na aceleração
        if (p->velX < P_VEL_X_MAX) {
            p->velX+=p->accX;
        }
    }
    // Se o player clicar o botão para andar para a esquerda
    if (IsKeyDown(KEY_A)){
        // Tira ele da escada e muda sua direção
        p->naEscada = 0;
        p->dir=ESQUERDA;
        // Se não está na sua velocidade máxima horizontal aumenta ela com base na aceleração
        if (p->velX > (P_VEL_X_MAX * -1)) {
            p->velX-=p->accX;
        }
    }
}

void processaPlayerNaEscada(PLAYER *p, char mapa[TILES][TILES]){
/* Essa função processa os movimentos do player na escada */
    // Se o player apertar o botão de subir a escada, testa se ele está em um dos grids contendo um elemento da escada e permite subir a escada
    if (IsKeyDown(KEY_W)){
        if (playerNaSubida(*p, mapa) || playerNaEscada(*p, mapa) || playerNaDescida(*p, mapa) || playerNaEscadaComPlataforma(*p, mapa)){
            // Atualiza o estado do player e centraliza ele na escada
            p->naEscada=1;
            centralizaPlayerNaEscada(p, mapa);
        }
        // Se o player estiver na escada e não estiver no final dela, sua posição vertical aumenta enquanto pressionar o botão de subida
        if (p->naEscada){
            if (!pixelDeCimaVazio(*p, mapa)){
                p->posY-=P_VEL_ESCADA;
            }
        }
    }
    // Se o player apertar o botão de descer a escada, testa se ele está em um dos grids contendo um elemento da escada e permite descer a escada
    if (IsKeyDown(KEY_S)){
        if (playerNaSubida(*p, mapa) || playerNaEscada(*p, mapa) || playerNaDescida(*p, mapa) || playerNaEscadaComPlataforma(*p, mapa)){
            // Atualiza o estado do player e centraliza ele na escada
            p->naEscada=1;
            centralizaPlayerNaEscada(p, mapa);
        }
        // Se o player estiver na escada e não estiver no final dela, sua posição vertical diminui enquanto pressionar o botão de descida
        if (p->naEscada && !playerNoChao(p, mapa)){
            p->posY+=P_VEL_ESCADA;
        }
    }
}

void processaPuloPlayer(PLAYER *p, char mapa[TILES][TILES], Sound sound){
/* Essa função processa o pulo do player */
    // Se o player apertar o botão de pular, tira ele da escada
    if (IsKeyPressed(KEY_SPACE)){
        p->naEscada=0;
        // Se o player estiver em algum lugar que pode pular, aplica os efeitos do pulo
        if (playerNoChao(p, mapa) || playerNaPlataforma(p, mapa)){
            PlaySound(sound);
            // Tira o player do chao para que nao fique preso, muda sua velocidade vertical e altera seu estado
            p->posY -= COMP_COLUNA/4;
            p->velY = P_VEL_PULO;
            p->noChao = 0;
        }
    }
}

void processaColisoesPlayer(PLAYER *p, char mapa[TILES][TILES]){
/* Essa função processa os movimentos do player ao colidir com elementos do cenário */
    // Se o player estiver em um chão ou plataforma (e não está no modo de subir escada) corrige sua posição vertical e zera sua velocidade vertical
    if (playerNoChao(p, mapa) || playerNaPlataforma(p, mapa) && !(*p).naEscada){
        corrigePersonagemY(p);
        p->velY=0;
    }
    // Se o player colidiu com o teto faz ele imediatamente cair
    else if (colidiuTeto(*p, mapa)){
        p->velY=GRAVIDADE;
    }
    // Se chegou em alguma borda ou parede, para o player e puxa ele para fora até ele sair completamente
    if (colidiuBordaDireita(*p) || colidiuParedeDireita(*p, mapa)){
        p->posX-=1;
        p->velX=0;
    }
    if (colidiuBordaEsquerda(*p) || colidiuParedeEsquerda(*p, mapa)){
        p->posX+=1;
        p->velX=0;
    }
}

int colidiuBordaEsquerda(PLAYER p){
/* Essa função testa se o player chegou no limite da borda esquerda */
    if (p.posX<0)
        return 1;
    else
        return 0;
}

int colidiuBordaDireita(PLAYER p){
/* Essa função testa se o player chegou no limite da borda direita */
    if ((p.posX+COMP_LINHA)>LARGURA)
        return 1;
    else
        return 0;
}

int colidiuMonstro(PLAYER p, MONSTRO monstros[10], int qtdMonstros){
/* Essa função testa se o player colidiu com algum dos monstros */
    int colidiu=0;
    // Transforma as posições do player em grid
    int posXEsq=p.posX/COMP_LINHA;
    int posXDir=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posY=p.posY/COMP_COLUNA;
    int posXEsqMonstro, posXDirMonstro, posYMonstro;

    // Testa com cada um dos monstros
    for (int i=0 ; i<qtdMonstros ; i++){
        // Transforma a posição do monstro em grid
        posXEsqMonstro=monstros[i].posX/COMP_LINHA;
        posXDirMonstro=(monstros[i].posX+COMP_LINHA-1)/COMP_LINHA;
        posYMonstro=monstros[i].posY/COMP_COLUNA;
        // Testa se o player está no mesmo grid que o monstro (se o monstro estiver vivo)
        if ((monstros[i].estadoMonstro==ATIVO) && (posY==posYMonstro) && ((posXDir==posXDirMonstro) || (posXEsq==posXDirMonstro) || (posXDir==posXEsqMonstro) || (posXEsq==posXEsqMonstro)))
            colidiu=1;
    }
    return colidiu;
}

int colidiuTeto(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player colidiu com o teto */
    // Transforma a posição do player em grid
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=(p.posY+1)/COMP_COLUNA;
    // Testa se essa posição no grid coincide com um chão
    if (mapa[posYGrid][posXGridEsquerda]=='Z' || mapa[posYGrid][posXGridDireita]=='Z')
        return 1;
    else
        return 0;
}

int colidiuParedeDireita(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player colidiu com uma parede em sua direita */
    // Transforma a posição do player em grid
    int posXGrid=(p.posX+COMP_LINHA+1)/COMP_LINHA;
    int posYGridCabeca=p.posY/COMP_COLUNA;
    int posYGridPe=(p.posY+COMP_COLUNA-1)/COMP_COLUNA;
    // Testa se essa posição no grid coincide com uma parede
    if (mapa[posYGridCabeca][posXGrid]=='Z' || mapa[posYGridPe][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int colidiuParedeEsquerda(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player colidiu com uma parede em sua esquerda */
    // Transforma a posição do player em grid
    int posXGrid=(p.posX-1)/COMP_LINHA;
    int posYGridCabeca=p.posY/COMP_COLUNA;
    int posYGridPe=(p.posY+COMP_COLUNA-1)/COMP_COLUNA;
    // Testa se essa posição no grid coincide com uma parede
    if (mapa[posYGridCabeca][posXGrid]=='Z' || mapa[posYGridPe][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int playerNoChao(PLAYER *p, char mapa[TILES][TILES]){
/* Essa função testa se o player está no chão */
    // Transforma a posição do player em grid
    int posXGridEsquerda=p->posX/COMP_LINHA;
    int posXGridDireita=(p->posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p->posY/COMP_COLUNA;
    // Testa se essa posição no grid coincide com um chão
    if (mapa[posYGrid+1][posXGridEsquerda]=='Z' || mapa[posYGrid+1][posXGridDireita]=='Z') {
        p->noChao = 1;
        return 1;
    }
    else
        return 0;
}

int playerNaPlataforma(PLAYER *p, char mapa[TILES][TILES]){
/* Essa função testa se o player está em uma plataforma */
    // Transforma a posição do player em grid
    int posXGridEsquerda=p->posX/COMP_LINHA;
    int posXGridDireita=(p->posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p->posY/COMP_COLUNA;
     // Testa se essa posição no grid coincide com uma plataforma
    if (mapa[posYGrid+1][posXGridEsquerda]=='X' || mapa[posYGrid+1][posXGridDireita]=='X') {
        p->noChao = 1;
        return 1;
    }
    else
        return 0;
}
int checaPlayerMapa(PLAYER p, char mapa[TILES][TILES], char ch){
/* Essa é uma função genérica que testa se no grid atual do player está o caractere de entrada */
    // Transforma a posição do player em grid
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;
    // Testa se essa posição no grid coincide com o caractere de entrada
    if (mapa[posYGrid][posXGridEsquerda]==ch || mapa[posYGrid][posXGridDireita]==ch)
        return 1;
    else
        return 0;
}

int playerNaSubida(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player está no grid de uma subida */
    return checaPlayerMapa(p, mapa, 'S');
}

int playerNaEscada(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player está no grid de uma escada */
    return checaPlayerMapa(p, mapa, 'H');
}

int playerNaDescida(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player está no grid de uma descida */
    return checaPlayerMapa(p, mapa, 'D');
}

int playerNaEscadaComPlataforma(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player está no grid de uma plataforma */
    return checaPlayerMapa(p, mapa, 'X');
}

int playerNoFinal(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se o player está no grid do final do jogo */
    return checaPlayerMapa(p, mapa, 'F');
}

int caiuDoMapa(PLAYER p){
/* Essa função testa se o player caiu do mapa */
    if (p.posY>ALTURA+CABECALHO)
        return 1;
    else
        return 0;
}

int pixelDeCimaVazio(PLAYER p, char mapa[TILES][TILES]){
/* Essa função testa se acima da cabeça do player há um espaço vazio */
    // Transforma a posição do player em grid
    int posXGrid=p.posX/COMP_LINHA;
    int posYGrid=(p.posY-1)/COMP_COLUNA;
    // Testa se essa posição no grid coincide com um espaço (vazio)
    if (mapa[posYGrid][posXGrid]==' ')
        return 1;
    else
        return 0;
}
