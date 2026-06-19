#include "monstro.h"
#include "raylib.h"
#include "funcoesGerais.h"

void iniciaMonstros(MONSTRO monstros[M_QTD_MAX], char mapa[TILES][TILES], int *qtdMonstros){
/* Essa função inicia a posição dos monstros baseado no mapa define suas velocidades iniciais randomicamente */
    // Variável usada para acessar e manipular o vetor dos monstros
    int k=0;
    // Percorre a matriz
    for (int i=0 ; i<TILES ; i++){
        for (int j=0 ; j<TILES ; j++){
            if (mapa[i][j]=='E'){
                // Se encontrar o monstro na matriz inicia sua posição e informações necessárias
                monstros[k].posX=j*COMP_LINHA;
                monstros[k].posY=i*COMP_COLUNA;
                monstros[k].estadoMonstro=ATIVO;
                monstros[k].animacaoTimer=0;
                monstros[k].frameAtual=0;

                // Decide aleatóriamente a direção que o monstro vai seguir, com positivo sendo para a direita e negativo para a esquerda
                if (GetRandomValue(0,1)==0)
                    monstros[k].velX=M_VEL_X;
                else
                    monstros[k].velX=-M_VEL_X;
                // Incrementa a variável para manipular a próxima posição do vetor
                k++;
            }
        }
    }
    // Atualiza a quantidade de monstros encontrados no mapa
    *qtdMonstros=k;
}

void regulaMovimentoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, char mapa[TILES][TILES]){
/* Essa função altera a direção do movimento dos monstros checando suas posições no mapa */
    // Itera um número de vezes igual a quantidade de monstros
    for (int i=0 ; i<qtdMonstros ; i++){
        if (monstros[i].estadoMonstro==ATIVO){
             // Verifica se o monstro está tocando a borda esquerda e altera sua direção se necessário
            if (monstros[i].posX-1<0)
                 monstros[i].velX*=-1;
            // Verifica se o monstro está se movendo para a direita
            else if (monstros[i].velX>0){
                // Verifica se tem chão na sua frente e se necessário muda sua direção
                if (!temChaoNaDireita(monstros[i], mapa))
                    monstros[i].velX*=-1;
            }
            // Se não ele está se movendo para a esquerda
            else
                // Verifica se tem chão na sua frente e se necessário muda sua direção
                if (!temChaoNaEsquerda(monstros[i], mapa))
                    monstros[i].velX*=-1;
        }
    }
}

void moveMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros){
/* Essa função muda a posição dos monstros no mapa */
    // Itera um número de vezes igual a quantidade de monstros
    for (int i=0 ; i<qtdMonstros ; i++){
        // Se o monstro estiver ativo muda sua posição usando sua velocidade
        if (monstros[i].estadoMonstro==ATIVO){
            monstros[i].posX+=monstros[i].velX;
        }
        // Atualiza qual a direção do monstro
        if (monstros[i].velX > 0)
            monstros[i].dir = DIREITA;
        else
            monstros[i].dir = ESQUERDA;
    }
}

int temChaoNaDireita(MONSTRO monstro, char mapa[TILES][TILES]){
/* Essa função verifica se há um chão na direita do monstro */
    // Transforma a posição do monstro em grid
    int posXGrid=monstro.posX/COMP_LINHA;
    int posYGrid=monstro.posY/COMP_COLUNA;
    // Verifica se logo abaixo e à direita possui um chão ou plataforma
    if (mapa[posYGrid+1][posXGrid+1]=='Z' || mapa[posYGrid+1][posXGrid+1]=='X')
        return 1;
    else
        return 0;
}

int temChaoNaEsquerda(MONSTRO monstro, char mapa[TILES][TILES]){
/* Essa função verifica se há um chão na esquerda do monstro */
    // Transforma a posição do monstro em grid
    int posXGrid=monstro.posX/COMP_LINHA;
    int posYGrid=monstro.posY/COMP_COLUNA;
    // Verifica se logo abaixo possui um chão ou plataforma
    if (mapa[posYGrid+1][posXGrid]=='Z' || mapa[posYGrid+1][posXGrid]=='X')
        return 1;
    else
        return 0;
}
