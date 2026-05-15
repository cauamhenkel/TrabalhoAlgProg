#include "monstro.h"

void iniciaMonstros(MONSTRO monstros[10], char mapa[30][30], int *qtdMonstros){
/* Essa função inicia a posição dos monstros baseado no mapa define suas velocidades iniciais randomicamente */
    int k=0;                                           // Variável usada para acessar e manipular o vetor dos monstros
    for (int i=0 ; i<30 ; i++){                        // Percorre a matriz
        for (int j=0 ; j<30 ; j++){
            if (mapa[i][j]=='E'){                      // Se encontrar a posição de um monstro no mapa
                monstros[k].posX=j*COMP_LINHA;         // Define a posição horizontal como o comprimento da linha multiplicado pelo numero da coluna da matriz
                monstros[k].posY=i*COMP_COLUNA;        // Define a posição vertical como o comprimento da coluna multiplicado pelo numero da linha da matriz

                if (GetRandomValue(0,1)==0)
                    monstros[k].velX=COMP_LINHA/10;    // Define a velocidade como positiva
                else
                    monstros[k].velX=-COMP_LINHA/10;   // Ou negativa

                k++;                                   // Incrementa a variável para manipular a próxima posição do vetor
            }
        }
    }
    *qtdMonstros=k;                                    // Altera a quantidade de monstros para a quantidade de monstros encontrados na verificação do mapa
}

void regulaMovimentoMonstros(MONSTRO monstros[10], int qtdMonstros, char mapa[30][30]){
/* Essa posição altera a direção do movimento dos monstros checando suas posições no mapa */
    for (int i=0 ; i<qtdMonstros ; i++){               // Itera um número de vezes igual a quantidade de monstros
        if (monstros[i].posX-1<0)                      // Verifica se o monstro está tocando a borda esquerda
            monstros[i].velX*=-1;                      // Se sim, muda sua direção
        else if (monstros[i].velX>0){                  // Verifica se o monstro está se movendo para a direita
            if (!temChaoNaDireita(monstros[i], mapa))  // Se sim, verifica se tem chão na sua frente
                monstros[i].velX*=-1;                  // Se não tiver, muda sua direção
        }
        else                                           // Se não, o monstro está se movendo para a esquerda
            if (!temChaoNaEsquerda(monstros[i], mapa)) // Verifica se tem chão na sua frente
                monstros[i].velX*=-1;                  // Se não tiver, muda sua direção
    }
}

void moveMonstros(MONSTRO monstros[10], int qtdMonstros){
/* Essa função muda a posição dos monstros */
    for (int i=0 ; i<qtdMonstros ; i++){    // Itera um número de vezes igual a quantidade de monstros
        monstros[i].posX+=monstros[i].velX; // Muda a posição do monstro usando sua velocidade
    }
}

void desenhaMonstros(MONSTRO monstros[10], int qtdMonstros){
/* Essa função desenha os monstros usando suas posições */
    for (int i=0 ; i<qtdMonstros ; i++){    // Itera um número de vezes igual a quantidade de monstros
        DrawRectangle(monstros[i].posX, monstros[i].posY+CABECALHO, COMP_LINHA, COMP_COLUNA, PURPLE);
    }
}

int temChaoNaDireita(MONSTRO monstro, char mapa[30][30]){
/* Essa função verifica se há um chão na direita do monstro */
    int posXGrid=monstro.posX/COMP_LINHA;   // Transforma a posição horizontal do monstro em um ponto da matriz
    int posYGrid=monstro.posY/COMP_COLUNA;  // Transforma a posição vertical do monstro em um ponto da matriz

    if (mapa[posYGrid+1][posXGrid+1]=='Z')  // Verifica se logo abaixo e à direita possui um chão
        return 1;                           // Se sim, retorna verdadeiro
    else
        return 0;                           // Se não, retorna falso
}

int temChaoNaEsquerda(MONSTRO monstro, char mapa[30][30]){
/* Essa função verifica se há um chão na esquerda do monstro */
    int posXGrid=monstro.posX/COMP_LINHA;   // Transforma a posição horizontal do monstro em um ponto da matriz
    int posYGrid=monstro.posY/COMP_COLUNA;  // Transforma a posição vertical do monstro em um ponto da matriz

    if (mapa[posYGrid+1][posXGrid]=='Z')    // Verifica se logo abaixo possui um chão, como a posição do monstro fica no canto superior esquerdo não precisa verificar mais para o lado
        return 1;                           // Se sim, retorna verdadeiro
    else
        return 0;                           // Se não, retorna falso
}
