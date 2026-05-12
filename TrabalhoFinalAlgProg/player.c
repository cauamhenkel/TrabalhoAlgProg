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
    (*p).saude=1;
}

void desenhaPlayer(PLAYER p){
    DrawRectangle(p.posX, p.posY+CABECALHO, COMP_LINHA, COMP_COLUNA, BLUE);
}

void sobeEscada(PLAYER *p, char mapa[30][30]){
    int posXGrid=(*p).posX/COMP_LINHA;
    int posYGrid=(*p).posY/COMP_COLUNA;
    int subirTiles=0;

    while(mapa[posYGrid][posXGrid]!='D'){
        subirTiles+=COMP_COLUNA;
        posYGrid--;
    }

    (*p).posY-=subirTiles;
}

void desceEscada(PLAYER *p, char mapa[30][30]){
    int posXGrid=(*p).posX/COMP_LINHA;
    int posYGrid=(*p).posY/COMP_COLUNA;
    int subirTiles=0;

    while(mapa[posYGrid][posXGrid]!='S'){
        subirTiles+=COMP_COLUNA;
        posYGrid++;
    }

    (*p).posY+=subirTiles;
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

int checaPlayerMapa(PLAYER p, char mapa[30][30], char ch){
    int posXGrid=p.posX/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;
    if (mapa[posYGrid][posXGrid]==ch)
        return 1;
    else
        return 0;
}

int playerNoChao(PLAYER p, char mapa[30][30]){
    int posXGrid=p.posX/COMP_LINHA;
    int posYGrid=p.posY/COMP_COLUNA;
    if (mapa[posYGrid+1][posXGrid]=='Z')
        return 1;
    else
        return 0;
}

int playerNaEscada(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'S');
}

int playerNaDescida(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'D');
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
