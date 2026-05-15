#include "funcoesGerais.h"

void arqMapa(char nomeMapa[12], int numMapa){
/* Essa função cria uma string com o nome do arquivo que será usado para criar o mapa da fase */
    char numString[3];                 // Vetor para guardar a string do número da fase

    for (int i=0 ; i<12; i++){         // Deixa a string zerada
        nomeMapa[i]=0;
    }

    sprintf(numString, "%d", numMapa); // Transforma o número de entrada em uma string
    strcat(nomeMapa, "mapa");          // Insere na string final a palavra mapa
    strcat(nomeMapa, numString);       // Insere na string final o número da fase
    strcat(nomeMapa, ".txt");          // Insere na string final o .txt
}

void criaMapa(char mapa[30][30], char *arq){
/* Essa função transforma um arquivo de .txt formatado para ser um mapa em uma matriz 30 por 30 */
    FILE *leitura;                           // Ponteiro usado para ler o arquivo
    char ch;                                 // Variável para salvar momentaneamente o caractere lido do arquivo

    if ((leitura = fopen(arq, "r")) == NULL) // Abre o arquivo de entrada no ponteiro e faz algo se a leitura der errado
        ;                                    // Ainda não foi definido nada caso a leitura der errado

    for (int i=0 ; i<30 ; i++){
        for (int j=0 ; j<30 ; j++){
            ch=fgetc(leitura);               // Lê o caractere do arquivo
            while (ch=='\n' || ch=='\r')     // Se o caractere lido for algum de formatação (como enter)
                ch=fgetc(leitura);           // Ignora e pega o próximo caractere, fica em loop até achar um caractere normal
            mapa[i][j]=ch;                   // Salva o caractere na matriz
        }
    }
    fclose(leitura);                         // Fecha o arquivo
}

void desenhaMapa(char mapa[30][30]){
/* Essa função desenha o mapa usando a matriz como referência */
    int posX=0, posY=0;              // Variáveis para desenhar os elementos do mapa
    for (int i=0 ; i<30 ; i++){
        posX=0;                      // Volta a posição horizontal para o início (começa desenhando da esquerda para a direita)
        for (int j=0 ; j<30 ; j++){
            switch(mapa[i][j]){      // Verifica o caractere atual
                case 'Z':            // Se for Z, desenha o chão
                    DrawRectangle(posX, posY+CABECALHO, COMP_LINHA, COMP_COLUNA, BROWN);
                    break;
                case 'S':            // Se for S, H ou D desenha a escada
                case 'H':
                case 'D':
                    DrawRectangle(posX, posY+CABECALHO, COMP_LINHA, COMP_COLUNA, ORANGE);
                    break;
                case 'F':            // Se for F desenha o final da fase
                    DrawRectangle(posX, posY+CABECALHO, COMP_LINHA, COMP_COLUNA, RED);
            }
            posX+=COMP_LINHA;        // Passa para a próxima posição horizontal
        }
        posY+=COMP_COLUNA;           // Após uma linha inteira desenhada, passa para a próxima linha
    }
}
