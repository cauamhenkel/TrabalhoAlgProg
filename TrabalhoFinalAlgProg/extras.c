#include "extras.h"

void criaProjetil(PLAYER p, PROJETIL *pr){
    pr->estado=ATIVO;
    if (p.dir==DIREITA){
        pr->posX=p.posX+COMP_LINHA;
        pr->posY=p.posY;
        pr->velX=PROJ_VEL_X;
    }
    else{
        pr->posX=p.posX-COMP_LINHA/5; // Por algum motivo precisa de mais do que 1 pixel pra sair de dentro do player na esquerda
        pr->posY=p.posY;
        pr->velX=-PROJ_VEL_X;
    }
}

void moveProjetil(PROJETIL *pr){
    if (pr->estado==ATIVO)
        pr->posX+=pr->velX;
}

void processaColisoesProjetil(PROJETIL *pr, MONSTRO monstros[M_QTD_MAX], char mapa[TILES][TILES]){
    int posXGrid=(pr->posX)/COMP_LINHA;
    int posYGrid=(pr->posY)/COMP_COLUNA;

    if ((pr->posX)<0 || (pr->posX)>LARGURA){
        pr->estado=DESATIVADO;
    }
    if (mapa[posYGrid][posXGrid]=='Z'){
        pr->estado=DESATIVADO;
    }
}

void mataMonstrosProjetil(PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros){
    for (int i=0 ; i<qtdMonstros ; i++){
        if ((pr->posX/COMP_LINHA==monstros[i].posX/COMP_LINHA) && (pr->posY/COMP_COLUNA==monstros[i].posY/COMP_COLUNA) && monstros[i].estadoMonstro==ATIVO){
            monstros[i].estadoMonstro=DESATIVADO;
            pr->estado=DESATIVADO;
        }
    }
}

void desenhaProjetil(PROJETIL pr){
    if (pr.estado==ATIVO)
        DrawRectangle(pr.posX, pr.posY+(COMP_COLUNA*2), COMP_LINHA/5, COMP_COLUNA/5, WHITE);
        // Por algum motivo precisa desenhar mais pra baixo a posição do tiro, senão ele sai voando
}
