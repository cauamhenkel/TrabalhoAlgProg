#include "monstro.h"

void iniciaMonstros(MONSTRO monstros[10], char mapa[30][30], int *qtdMonstros){
    int k=0;
    for (int i=0 ; i<30 ; i++){
        for (int j=0 ; j<30 ; j++){
            if (mapa[i][j]=='E'){
                monstros[k].posX=j*COMP_LINHA;
                monstros[k].posY=i*COMP_COLUNA;

                (*qtdMonstros)++;

                if (GetRandomValue(0,1)==0)
                    monstros[k].velX=COMP_LINHA/10;
                else
                    monstros[k].velX=-COMP_LINHA/10;

                k++;
            }
        }
    }
}

void regulaMovimentoMonstros(MONSTRO monstros[10], int qtdMonstros, char mapa[30][30]){
    for (int i=0 ; i<qtdMonstros ; i++){
        if (monstros[i].posX-1<0)
            monstros[i].velX*=-1;
        else if (monstros[i].velX>0){
            if (!temChaoNaDireita(monstros[i], mapa))
                monstros[i].velX*=-1;
        }
        else
            if (!temChaoNaEsquerda(monstros[i], mapa))
                monstros[i].velX*=-1;
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

int temChaoNaDireita(MONSTRO monstro, char mapa[30][30]){
    int posXGrid=monstro.posX/COMP_LINHA;
    int posYGrid=monstro.posY/COMP_COLUNA;

    if (mapa[posYGrid+1][posXGrid+1]=='Z')
        return 1;
    else
        return 0;
}

int temChaoNaEsquerda(MONSTRO monstro, char mapa[30][30]){
    int posXGrid=monstro.posX/COMP_LINHA;
    int posYGrid=monstro.posY/COMP_COLUNA;

    if (mapa[posYGrid+1][posXGrid]=='Z')
        return 1;
    else
        return 0;
}
