#include "monstro.h"

void iniciaMonstros(MONSTRO monstros[10], char mapa[30][30]){
    int k=0;
    for (int i=0 ; i<30 ; i++){
        for (int j=0 ; j<30 ; j++){
            if (mapa[i][j]=='E'){
                monstros[k].posX=j*COMP_LINHA;
                monstros[k].posY=i*COMP_COLUNA;
                k++;
            }
        }
    }
}

void moveMonstros(MONSTRO monstros[10], int qtdMonstros){
    for (int i=0 ; i<qtdMonstros ; i++){
        monstros[i].posX+=monstros[i].velX;
    }
}

void desenhaMonstros(MONSTRO monstros[10], int qtdMonstros){
    for (int i=0 ; i<qtdMonstros ; i++){
        DrawRectangle(monstros[i].posX, monstros[i].posY+CABECALHO, COMP_LINHA, COMP_COLUNA, PURPLE);
    }
}

int calculaQtdMonstros(char mapa[30][30]){
    int  acc=0;
    for (int i=0 ; i<30 ; i++){
        for (int j=0; j<30 ; j++){
            if (mapa[i][j]=='E'){
                acc++;
            }
        }
    }
    return acc;
}
