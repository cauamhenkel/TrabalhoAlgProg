#include "monstro.h"
#include "funcoesGerais.h"
#include "raylib.h"

void iniciaMonstros(MONSTRO monstros[M_QTD_MAX], char mapa[TILES][TILES], int *qtdMonstros){
/* Essa fun��o inicia a posi��o dos monstros baseado no mapa define suas velocidades iniciais randomicamente */
    int k=0;                                           // Vari�vel usada para acessar e manipular o vetor dos monstros
    for (int i=0 ; i<TILES ; i++){                        // Percorre a matriz
        for (int j=0 ; j<TILES ; j++){
            if (mapa[i][j]=='E'){                      // Se encontrar a posi��o de um monstro no mapa
                monstros[k].posX=j*COMP_LINHA;         // Define a posi��o horizontal como o comprimento da linha multiplicado pelo numero da coluna da matriz
                monstros[k].posY=i*COMP_COLUNA;        // Define a posi��o vertical como o comprimento da coluna multiplicado pelo numero da linha da matriz
                monstros[k].estadoMonstro=ATIVO;

                if (GetRandomValue(0,1)==0)
                    monstros[k].velX=M_VEL_X;    // Define a velocidade como positiva
                else
                    monstros[k].velX=-M_VEL_X;   // Ou negativa

                k++;                                   // Incrementa a vari�vel para manipular a pr�xima posi��o do vetor
            }
        }
    }
    *qtdMonstros=k;                                    // Altera a quantidade de monstros para a quantidade de monstros encontrados na verifica��o do mapa
}

void regulaMovimentoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, char mapa[TILES][TILES]){
/* Essa posi��o altera a dire��o do movimento dos monstros checando suas posi��es no mapa */
    for (int i=0 ; i<qtdMonstros ; i++){                   // Itera um n�mero de vezes igual a quantidade de monstros
        if (monstros[i].estadoMonstro==ATIVO){
            if (monstros[i].posX-1<0)                      // Verifica se o monstro est� tocando a borda esquerda
                monstros[i].velX*=-1;                      // Se sim, muda sua dire��o
            else if (monstros[i].velX>0){                  // Verifica se o monstro est� se movendo para a direita
                if (!temChaoNaDireita(monstros[i], mapa))  // Se sim, verifica se tem ch�o na sua frente
                    monstros[i].velX*=-1;                  // Se n�o tiver, muda sua dire��o
            }
            else                                           // Se n�o, o monstro est� se movendo para a esquerda
                if (!temChaoNaEsquerda(monstros[i], mapa)) // Verifica se tem ch�o na sua frente
                    monstros[i].velX*=-1;                  // Se n�o tiver, muda sua dire��o
        }
    }
}

void moveMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros){
/* Essa fun��o muda a posi��o dos monstros */
    for (int i=0 ; i<qtdMonstros ; i++){        // Itera um n�mero de vezes igual a quantidade de monstros
        if (monstros[i].estadoMonstro==ATIVO){
            monstros[i].posX+=monstros[i].velX; // Muda a posi��o do monstro usando sua velocidade
        }

        if (monstros[i].velX > 0)
            monstros[i].dir = DIREITA;
        else
            monstros[i].dir = ESQUERDA;
    }
}

void desenhaMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, Texture2D sprite){
/* Essa fun��o desenha os monstros usando suas posi��es */
Rectangle fonte; // Retangulo referente ao sprite original

    for (int i=0 ; i<qtdMonstros ; i++){    // Itera um n�mero de vezes igual a quantidade de monstros
        if (monstros[i].estadoMonstro==ATIVO){
            if (monstros[i].dir == ESQUERDA)
                fonte = (Rectangle){0, 0, sprite.width, sprite.height}; // Retangulo referente ao sprite original
            else
                fonte = (Rectangle){sprite.width, 0, -sprite.width, sprite.height}; // Retangulo referente ao sprite original (flippado horizontamente)
            Rectangle destino = {monstros[i].posX, monstros[i].posY + CABECALHO, COMP_LINHA, COMP_COLUNA}; // Retangulo referente ao sprite exibido na tela
            DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, PURPLE);
        }
    }
}

int temChaoNaDireita(MONSTRO monstro, char mapa[TILES][TILES]){
/* Essa fun��o verifica se h� um ch�o na direita do monstro */
    int posXGrid=monstro.posX/COMP_LINHA;   // Transforma a posi��o horizontal do monstro em um ponto da matriz
    int posYGrid=monstro.posY/COMP_COLUNA;  // Transforma a posi��o vertical do monstro em um ponto da matriz

    if (mapa[posYGrid+1][posXGrid+1]=='Z' || mapa[posYGrid+1][posXGrid+1]=='X')  // Verifica se logo abaixo e � direita possui um ch�o ou plataforma
        return 1;                           // Se sim, retorna verdadeiro
    else
        return 0;                           // Se n�o, retorna falso
}

int temChaoNaEsquerda(MONSTRO monstro, char mapa[TILES][TILES]){
/* Essa fun��o verifica se h� um ch�o na esquerda do monstro */
    int posXGrid=monstro.posX/COMP_LINHA;   // Transforma a posi��o horizontal do monstro em um ponto da matriz
    int posYGrid=monstro.posY/COMP_COLUNA;  // Transforma a posi��o vertical do monstro em um ponto da matriz

    if (mapa[posYGrid+1][posXGrid]=='Z' || mapa[posYGrid+1][posXGrid]=='X')    // Verifica se logo abaixo possui um ch�o ou plataforma, como a posi��o do monstro fica no canto superior esquerdo n�o precisa verificar mais para o lado
        return 1;                           // Se sim, retorna verdadeiro
    else
        return 0;                           // Se n�o, retorna falso
}
