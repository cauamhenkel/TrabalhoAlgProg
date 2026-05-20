#include "player.h"

void iniciaPlayer(PLAYER *p, char mapa[30][30]){
    for (int i=0 ; i<30 ; i++){
        for (int j=0 ; j<30 ; j++){
            if (mapa[i][j]=='P'){
                (*p).posX=j*COMP_LINHA; // j para linha
                (*p).posY=i*COMP_COLUNA; // i para coluna
            }
        }
    }
    (*p).velX=COMP_COLUNA/5;
    (*p).invencibilidade=0;
}

void desenhaPlayer(PLAYER p){
    if (p.invencibilidade % 10 > 5) // Player pisca quando toma dano
        DrawRectangle(p.posX, p.posY+CABECALHO, COMP_LINHA, COMP_COLUNA, RED);
    else
        DrawRectangle(p.posX, p.posY+CABECALHO, COMP_LINHA, COMP_COLUNA, BLUE);
}

void exibeSaude(PLAYER p) {
    DrawText(TextFormat("Saude: %d", p.saude), 10, 10, 20, RED);
}

void gravidade(PLAYER *p, int *terco){
    if (*terco%3==0){
        (*p).accY=1;
        if ((*p).velY<COMP_COLUNA){
            (*p).velY+=(*p).accY;
        }
        (*p).posY+=(*p).velY;
    }
    *terco++;
}

void corrigePersonagem(PLAYER *p){
    (*p).posY/=COMP_COLUNA;
    (*p).posY*=COMP_COLUNA;
}

void danoPlayer(PLAYER *p){
    (*p).saude--;
    (*p).invencibilidade = FPS;
}

int colidiuBordaEsquerda(PLAYER p){
    if (p.posX-1<0)
        return 1;
    else
        return 0;
}

int colidiuBordaDireita(PLAYER p){
    if ((p.posX+COMP_LINHA+1)>LARGURA)
        return 1;
    else
        return 0;
}

int colidiuMonstro(PLAYER p, MONSTRO monstros[10], int qtdMonstros){
    int colidiu=0;
    for (int i=0 ; i<qtdMonstros ; i++){
        if ((p.posX/COMP_LINHA==monstros[i].posX/COMP_LINHA) && (p.posY/COMP_COLUNA==monstros[i].posY/COMP_COLUNA))
            colidiu=1;
        else if (((p.posX/COMP_LINHA)+1==monstros[i].posX/COMP_LINHA) && (p.posY/COMP_COLUNA==monstros[i].posY/COMP_COLUNA))
            colidiu=1;
    }
    return colidiu;
}

int colidiuTeto(PLAYER p, char mapa[30][30]){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=(p.posY+1)/COMP_COLUNA;

    if (mapa[posYGrid][posXGridEsquerda]=='Z' || mapa[posYGrid][posXGridDireita]=='Z')
        return 1;
    else
        return 0;
}

int colidiuParedeDireita(PLAYER p, char mapa[30][30]){
    int posXGrid=(p.posX+COMP_LINHA+1)/COMP_LINHA;
    int posYGridCabeca=p.posY/COMP_COLUNA;
    int posYGridPe=(p.posY+COMP_COLUNA-1)/COMP_COLUNA;

    if (mapa[posYGridCabeca][posXGrid]=='Z' || mapa[posYGridPe][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int colidiuParedeEsquerda(PLAYER p, char mapa[30][30]){
    int posXGrid=(p.posX-1)/COMP_LINHA;
    int posYGridCabeca=p.posY/COMP_COLUNA;
    int posYGridPe=(p.posY+COMP_COLUNA-1)/COMP_COLUNA;

    if (mapa[posYGridCabeca][posXGrid]=='Z' || mapa[posYGridPe][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int checaPlayerMapa(PLAYER p, char mapa[30][30], char ch){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;

    if (mapa[posYGrid][posXGridEsquerda]==ch || mapa[posYGrid][posXGridDireita]==ch)
        return 1;
    else
        return 0;
}

int playerNoChao(PLAYER p, char mapa[30][30]){
    int posXGridEsquerda=p.posX/COMP_LINHA;
    int posXGridDireita=(p.posX+COMP_LINHA-1)/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;

    if (mapa[posYGrid+1][posXGridEsquerda]=='Z' || mapa[posYGrid+1][posXGridDireita]=='Z')
        return 1;
    else
        return 0;
}

int playerNaSubida(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'S');
}

int playerNaEscada(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'H');
}

int playerNaDescida(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'D');
}

int playerNoFinal(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'F');
}

int caiuDoMapa(PLAYER p){
    if (p.posY>ALTURA+CABECALHO)
        return 1;
    else
        return 0;
}

int pixelDeCimaVazio(PLAYER p, char mapa[30][30]){
    int posXGrid=p.posX/COMP_LINHA;
    int posYGrid=(p.posY-1)/COMP_COLUNA;

    if (mapa[posYGrid][posXGrid]==' ')
        return 1;
    else
        return 0;
}
