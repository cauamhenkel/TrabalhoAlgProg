#include "extras.h"
#include "raylib.h"

void criaProjetil(PLAYER p, PROJETIL *pr){
    pr->estado=ATIVO;
    pr->posY=p.posY+COMP_COLUNA/2;
    if (p.dir==DIREITA){
        pr->posX=p.posX+COMP_LINHA;
        pr->velX=PROJ_VEL_X;
    }
    else{
        pr->posX=p.posX-COMP_LINHA/5; // Por algum motivo precisa de mais do que 1 pixel pra sair de dentro do player na esquerda
        pr->velX=-PROJ_VEL_X;
    }
}

void moveProjetil(PROJETIL *pr){
    if (pr->estado==ATIVO)
        pr->posX+=pr->velX;
}

void processaColisoesProjetil(PROJETIL *pr, char mapa[TILES][TILES]){
    int posXGrid=(pr->posX)/COMP_LINHA;
    int posYGrid=(pr->posY)/COMP_COLUNA;

    if ((pr->posX)<0 || (pr->posX)>LARGURA){
        pr->estado=DESATIVADO;
    }
    if (mapa[posYGrid][posXGrid]=='Z'){
        pr->estado=DESATIVADO;
    }
}

void processaProjetil(PLAYER *p, PROJETIL *pr, char mapa[TILES][TILES]){
    if (IsKeyPressed(KEY_K) && !p->naEscada && p->qtdTiros>0 && p->cooldownTiro==0){
        criaProjetil(*p, pr);
        p->cooldownTiro=P_COOLDOWN_TIRO;

        if (p->qtdTiros>0){
            p->qtdTiros--;
        }
    }
    if (pr->estado==ATIVO){
        moveProjetil(pr);
        processaColisoesProjetil(pr, mapa);
    }
    if (p->cooldownTiro>0){
        p->cooldownTiro--;
    }
}

Vector2 mataMonstrosProjetil(PLAYER *p, PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros, int *pontos){
    int poXPr=pr->posX/COMP_LINHA;
    int posYPr=pr->posY/COMP_COLUNA;
    int posXMonstroEsq, posXMonstroDir;
    int posYMonstro;
    for (int i=0 ; i<qtdMonstros ; i++){
        posXMonstroEsq=monstros[i].posX/COMP_LINHA;
        posXMonstroDir=(monstros[i].posX+COMP_LINHA)/COMP_LINHA;
        posYMonstro=monstros[i].posY/COMP_COLUNA;

        if ((poXPr==posXMonstroEsq || poXPr==posXMonstroDir) && posYPr==posYMonstro && monstros[i].estadoMonstro==ATIVO && pr->estado==ATIVO){
            monstros[i].estadoMonstro=DESATIVADO;
            pr->estado=DESATIVADO;
            p->qtdTiros++;
            *pontos+=500;
            return (Vector2){monstros[i].posX, monstros[i].posY + COMP_COLUNA}; // Retorna a posicao em que o monstro morreu
        }
    }
    return (Vector2){-1.0f, -1.0f};  // Retorna uma posicao fora da tela caso nenhum monstro tenha morrido
}

void desenhaPontosSubindo(Vector2 vec) {
    DrawText("500", vec.x, vec.y, COMP_LINHA * 2 / 3, WHITE);
}

void desenhaProjetil(PROJETIL pr){
    if (pr.estado==ATIVO)
        DrawRectangle(pr.posX, pr.posY+(COMP_COLUNA*1.5), COMP_LINHA/5, COMP_COLUNA/5, WHITE);
        // Por algum motivo precisa desenhar mais pra baixo a posição do tiro, senão ele sai voando
}
