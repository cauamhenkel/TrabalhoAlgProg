#include "player.h"

void iniciaPlayer(PLAYER *p, char mapa[TILES][TILES]){
    for (int i=0 ; i<TILES ; i++){
        for (int j=0 ; j<TILES ; j++){
            if (mapa[i][j]=='P'){
                (*p).posX=j*COMP_LINHA; // j para linha
                (*p).posY=i*COMP_COLUNA; // i para coluna
            }
        }
    }
    p->dir=DIREITA;
    p->velX=0;
    p->velY=0;
    p->accX=P_ACC_X;
    p->invencibilidade=0;
    p->naEscada=0;
    p->qtdTiros=P_QTD_TIROS;
    p->cooldownTiro=0;
}

void desenhaPlayer(PLAYER p, Texture2D sprite){
    Rectangle fonte = {0, 0, sprite.width, sprite.height}; // Retangulo referente ao sprite original
    Rectangle destino = {p.posX, p.posY + CABECALHO, COMP_LINHA, COMP_COLUNA}; // Retangulo referente ao sprite exibido na tela

    if (p.invencibilidade % 10 > 5) // Player pisca quando toma dano
        DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, RED);
    else
        DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, WHITE); // Funcao escala automaticamente o retangulo da fonte para o do destino
}

void controlaGravidadePlayer(PLAYER *p, char mapa[TILES][TILES]) {
    if (p->naEscada || playerNoChao(*p, mapa) || playerNaPlataforma(*p, mapa)) {
        p->afetadoGravidade = 0;
    }
    else
        p->afetadoGravidade = 1;
}

void processaGravidadePlayer(PLAYER *p){
    if (p->afetadoGravidade) {
        p->accY = GRAVIDADE;

        if (p->velY<P_VEL_Y_MAX){
            p->velY+=p->accY;
        }
    }
}

void processaAceleracaoPlayer(PLAYER *p) {
    p->posX += p->velX;
    p->posY += p->velY;
}

void processaAtritoPlayer(PLAYER *p) {
    if (p->velX > 0) {
        p->velX -= FORCA_ATRITO;
    }
    if (p->velX < 0) {
        p->velX += FORCA_ATRITO;
    }
}

void corrigePersonagemY(PLAYER *p){
    p->posY/=COMP_COLUNA;
    p->posY*=COMP_COLUNA;
}

void centralizaPlayerNaEscada(PLAYER *p, char mapa[TILES][TILES]){
    int posXGrid=p->posX/COMP_LINHA;
    int posYGrid=p->posY/COMP_COLUNA;

    if (mapa[posYGrid][posXGrid]=='S' || mapa[posYGrid][posXGrid]=='H' || mapa[posYGrid][posXGrid]=='D' || mapa[posYGrid][posXGrid]=='X'){
        p->velX=0;
        p->velY=0; // "Locka" o player na escada
        p->posX/=COMP_LINHA;
        p->posX*=COMP_LINHA;
    }
    else if (mapa[posYGrid][posXGrid+1]=='S' || mapa[posYGrid][posXGrid+1]=='H' || mapa[posYGrid][posXGrid+1]=='D' || mapa[posYGrid][posXGrid+1]=='X'){
        p->velX=0; // "Locka" o player na escada
        p->velY=0;
        p->posX/=COMP_LINHA;
        p->posX*=COMP_LINHA;
        p->posX+=COMP_LINHA;
    }
}

void danoPlayer(PLAYER *p){
    p->saude--;
    p->invencibilidade = FPS;
}

void processaMovimentoPlayer(PLAYER *p, char mapa[TILES][TILES]) {
    if (IsKeyDown(KEY_D)){
        p->naEscada = 0;
        p->dir=DIREITA;

        if (p->velX < P_VEL_X_MAX) {
            p->velX+=p->accX;
        }
    }
    if (IsKeyDown(KEY_A)){
        p->naEscada = 0;
        p->dir=ESQUERDA;

        if (p->velX > (P_VEL_X_MAX * -1)) {
            p->velX-=p->accX;
        }
    }
}

void processaPlayerNaEscada(PLAYER *p, char mapa[TILES][TILES]) {
    if (IsKeyDown(KEY_W)){
        if (playerNaSubida(*p, mapa) || playerNaEscada(*p, mapa) || playerNaDescida(*p, mapa) || playerNaEscadaComPlataforma(*p, mapa)){
            p->naEscada=1;
            centralizaPlayerNaEscada(p, mapa);
        }
        if (p->naEscada){
            if (!pixelDeCimaVazio(*p, mapa)){
                p->posY-=P_VEL_ESCADA;
            }
        }
    }
    if (IsKeyDown(KEY_S)){
        if (playerNaSubida(*p, mapa) || playerNaEscada(*p, mapa) || playerNaDescida(*p, mapa) || playerNaEscadaComPlataforma(*p, mapa)){
            p->naEscada=1;
            centralizaPlayerNaEscada(p, mapa);
        }
        if (p->naEscada && !playerNoChao(*p, mapa)){
            p->posY+=P_VEL_ESCADA;
        }
    }
}

void processaPuloPlayer(PLAYER *p, char mapa[TILES][TILES]) {
    if (IsKeyPressed(KEY_SPACE)){
        p->naEscada=0;
        if (playerNoChao(*p, mapa) || playerNaPlataforma(*p, mapa)){
            p->posY -= COMP_COLUNA/4; // Tira o player do chao para que nao fique preso
            p->velY = P_VEL_PULO;
        }
    }
}

void processaColisoesPlayer(PLAYER *p, char mapa[TILES][TILES]) {
    if (playerNoChao(*p, mapa) || playerNaPlataforma(*p, mapa) && !(*p).naEscada){
        corrigePersonagemY(p);
        p->velY=0;
    }
    else if (colidiuTeto(*p, mapa)){
        p->velY=GRAVIDADE;
    }

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
    if (p.posX<0)
        return 1;
    else
        return 0;
}

int colidiuBordaDireita(PLAYER p){
    if ((p.posX+COMP_LINHA)>LARGURA)
        return 1;
    else
        return 0;
}

int colidiuMonstro(PLAYER p, MONSTRO monstros[10], int qtdMonstros){
    int colidiu=0;
    for (int i=0 ; i<qtdMonstros ; i++){
        if ((p.posX/COMP_LINHA==monstros[i].posX/COMP_LINHA) && (p.posY/COMP_COLUNA==monstros[i].posY/COMP_COLUNA) && monstros[i].estadoMonstro==ATIVO)
            colidiu=1;
        else if (((p.posX/COMP_LINHA)+1==monstros[i].posX/COMP_LINHA) && (p.posY/COMP_COLUNA==monstros[i].posY/COMP_COLUNA) && monstros[i].estadoMonstro==ATIVO)
            colidiu=1;
    }
    return colidiu;
}

int colidiuTeto(PLAYER p, char mapa[TILES][TILES]){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=(p.posY+1)/COMP_COLUNA;

    if (mapa[posYGrid][posXGridEsquerda]=='Z' || mapa[posYGrid][posXGridDireita]=='Z')
        return 1;
    else
        return 0;
}

int colidiuParedeDireita(PLAYER p, char mapa[TILES][TILES]){
    int posXGrid=(p.posX+COMP_LINHA+1)/COMP_LINHA;
    int posYGridCabeca=p.posY/COMP_COLUNA;
    int posYGridPe=(p.posY+COMP_COLUNA-1)/COMP_COLUNA;

    if (mapa[posYGridCabeca][posXGrid]=='Z' || mapa[posYGridPe][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int colidiuParedeEsquerda(PLAYER p, char mapa[TILES][TILES]){
    int posXGrid=(p.posX-1)/COMP_LINHA;
    int posYGridCabeca=p.posY/COMP_COLUNA;
    int posYGridPe=(p.posY+COMP_COLUNA-1)/COMP_COLUNA;

    if (mapa[posYGridCabeca][posXGrid]=='Z' || mapa[posYGridPe][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int playerNoChao(PLAYER p, char mapa[TILES][TILES]){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;

    if (mapa[posYGrid+1][posXGridEsquerda]=='Z' || mapa[posYGrid+1][posXGridDireita]=='Z')
        return 1;
    else
        return 0;
}

int playerNaPlataforma(PLAYER p, char mapa[TILES][TILES]){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;

    if (mapa[posYGrid+1][posXGridEsquerda]=='X' || mapa[posYGrid+1][posXGridDireita]=='X')
        return 1;
    else
        return 0;
}
int checaPlayerMapa(PLAYER p, char mapa[TILES][TILES], char ch){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;

    if (mapa[posYGrid][posXGridEsquerda]==ch || mapa[posYGrid][posXGridDireita]==ch)
        return 1;
    else
        return 0;
}

int playerNaSubida(PLAYER p, char mapa[TILES][TILES]){
    return checaPlayerMapa(p, mapa, 'S');
}

int playerNaEscada(PLAYER p, char mapa[TILES][TILES]){
    return checaPlayerMapa(p, mapa, 'H');
}

int playerNaDescida(PLAYER p, char mapa[TILES][TILES]){
    return checaPlayerMapa(p, mapa, 'D');
}

int playerNaEscadaComPlataforma(PLAYER p, char mapa[TILES][TILES]){
    return checaPlayerMapa(p, mapa, 'X');
}

int playerNoFinal(PLAYER p, char mapa[TILES][TILES]){
    return checaPlayerMapa(p, mapa, 'F');
}

int caiuDoMapa(PLAYER p){
    if (p.posY>ALTURA+CABECALHO)
        return 1;
    else
        return 0;
}

int pixelDeCimaVazio(PLAYER p, char mapa[TILES][TILES]){
    int posXGrid=p.posX/COMP_LINHA;
    int posYGrid=(p.posY-1)/COMP_COLUNA;

    if (mapa[posYGrid][posXGrid]==' ')
        return 1;
    else
        return 0;
}
