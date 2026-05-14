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
    (*p).saude=3;
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

void matarPlayer(PLAYER *p, EstadoJogo *ej, int *iniciouMapa, int *iniciouPlayer, int *iniciouMonstros){
    *ej = MENU;
    (*iniciouMapa)=0;
    (*iniciouPlayer)=0;
    (*iniciouMonstros)=0;
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

int playerNoFinal(PLAYER p, char mapa[30][30]){
    return checaPlayerMapa(p, mapa, 'F');
}

int caiuDoMapa(PLAYER p){
    if (p.posY>ALTURA+CABECALHO)
        return 1;
    else
        return 0;
}
