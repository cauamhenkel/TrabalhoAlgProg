#include "raylib.h"
#include "extras.h"
#include "graficos.h"

void criaProjetil(PLAYER p, PROJETIL *pr){
/* Essa função cria um projetil com base na posição do player */
    // Muda o estado e coloca sua posição Y
    pr->estado=ATIVO;
    pr->posY=p.posY+COMP_COLUNA/4;
    // Se o player estiver virado para a direita, cria na direita do player
    if (p.dir==DIREITA){
        pr->posX=p.posX+COMP_LINHA;
        pr->velX=PROJ_VEL_X;
    }
    // Se o player estiver virado para a esquerda, cria na esquerda do player
    else{
        // Por algum motivo precisa de mais do que 1 pixel pra sair de dentro do player na esquerda
        pr->posX=p.posX-COMP_LINHA/5;
        pr->velX=-PROJ_VEL_X;
    }
}

void moveProjetil(PROJETIL *pr){
/* Essa função muda a posição do tiro */
    // Muda apenas se ele estiver ativo
    if (pr->estado==ATIVO)
        pr->posX+=pr->velX;
}

void processaColisoesProjetil(PROJETIL *pr, char mapa[TILES][TILES]){
/* Essa função testa se o tiro saiu do mapa ou se bateu em uma parede, se sim, desativa o projetil */
    // Transforma a posição do projetil em uma posição no grid
    int posXGrid=(pr->posX)/COMP_LINHA;
    int posYGrid=(pr->posY)/COMP_COLUNA;
    // Testa se o tiro ainda está dentro da área do mapa
    if ((pr->posX)<0 || (pr->posX)>LARGURA){
        pr->estado=DESATIVADO;
    }
    // Testa se o tiro colidiu com alguma parede
    if (mapa[posYGrid][posXGrid]=='Z'){
        pr->estado=DESATIVADO;
    }
}

void processaProjetil(PLAYER *p, PROJETIL *pr, char mapa[TILES][TILES], Soundtrack sounds){
/* Essa função processa o funcionamento do projetil */
    // Testa se o jogador tentou atirar, impede se ele estiver na escada, sem munição ou se ainda está no cooldown
    if (IsKeyPressed(KEY_K) && !p->naEscada && p->qtdTiros>0 && p->cooldownTiro==0){
        // Cria o projetil no local
        criaProjetil(*p, pr);
        p->cooldownTiro=P_COOLDOWN_TIRO;
        PlaySound(sounds.tiro);
        // Reduz a quantidade de munição
        if (p->qtdTiros>0){
            p->qtdTiros--;
        }
    }
    // Age sobre o projetil apenas se ele está ativo
    if (pr->estado==ATIVO){
        moveProjetil(pr);
        processaColisoesProjetil(pr, mapa);
    }
    // Atualiza o cooldown
    if (p->cooldownTiro>0){
        p->cooldownTiro--;
    }
}

void desenhaPontosSubindo(Vector2 vec){
/* Essa função desenha os pontos acima do monstro morto depois que o player o mata */
    DrawText(TextFormat("%d", QTD_PONTOS_GANHO_DOS_INIMIGOS), vec.x, vec.y, COMP_LINHA * 2 / 3, WHITE);
}

void desenhaProjetil(PROJETIL pr){
/* Essa função desenha o projetil na tela */
    // Desenha apenas se ele estiver ativo
    if (pr.estado==ATIVO)
        // Por algum motivo precisa desenhar mais pra baixo a posição do tiro, senão ele sai voando
        DrawRectangle(pr.posX, pr.posY+CABECALHO, COMP_LINHA/5, COMP_COLUNA/5, WHITE);
}

Vector2 mataMonstrosProjetil(PLAYER *p, PROJETIL *pr, MONSTRO monstros[10], int qtdMonstros, int *qtdMonstrosMortos, int *pontos){
/* Essa função mata o monstro caso o projetil colida com ele */
    // Funções para transformar as posições em grid e facilitar os cálculos
    int poXPr=pr->posX/COMP_LINHA;
    int posYPr=pr->posY/COMP_COLUNA;
    int posXMonstroEsq, posXMonstroDir;
    int posYMonstro;
    // Itera sobre todos os montros
    for (int i=0 ; i<qtdMonstros ; i++){
        // Salva a posição do monstro no grid, testa os 2 extremos do monstro para maior precisão
        posXMonstroEsq=monstros[i].posX/COMP_LINHA;
        posXMonstroDir=(monstros[i].posX+COMP_LINHA)/COMP_LINHA;
        posYMonstro=monstros[i].posY/COMP_COLUNA;
        // Se o projetil estiver ativo e colidir com um monstro, mata ele e desativa o projetil
        if ((poXPr==posXMonstroEsq || poXPr==posXMonstroDir) && posYPr==posYMonstro && monstros[i].estadoMonstro==ATIVO && pr->estado==ATIVO){
            monstros[i].estadoMonstro=DESATIVADO;
            pr->estado=DESATIVADO;
            // O player recebe uma munição extra
            p->qtdTiros++;
            // O player recebe pontos por matar o monstro e aumenta o acumulador de monstros mortos por ele
            *pontos+=QTD_PONTOS_GANHO_DOS_INIMIGOS;
            (*qtdMonstrosMortos)++;
            // Retorna a posicao em que o monstro morreu
            return (Vector2){monstros[i].posX, monstros[i].posY + COMP_COLUNA};
        }
    }
    // Retorna uma posicao fora da tela caso nenhum monstro tenha morrido
    return (Vector2){-1.0f, -1.0f};
}
