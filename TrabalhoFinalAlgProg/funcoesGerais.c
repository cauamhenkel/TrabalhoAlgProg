#include "funcoesGerais.h"
#include "raylib.h"

void arqMapa(char nomeMapa[12], const char *nMapa){
    for (int i=0 ; i<12; i++){
        nomeMapa[i]=0;
    }
    strcat(nomeMapa, "mapa");
    strcat(nomeMapa, nMapa);
    strcat(nomeMapa, ".txt");
}

void criaMapa(char mapa[30][30], char *arq){
    FILE *leitura;
    char ch;

    if ((leitura = fopen(arq, "r")) == NULL)
        printf("Erro de abertura");

    for (int i=0 ; i<30 ; i++){
        for (int j=0 ; j<30 ; j++){
            ch=fgetc(leitura);
            while (ch=='\n' || ch=='\r')
                ch=fgetc(leitura);
            mapa[i][j]=ch;
        }
    }
    fclose(leitura);
}

void desenhaMapa(char mapa[30][30]){
    int posX=0, posY=0;
    for (int i=0 ; i<30 ; i++){
        posX=0;
        for (int j=0 ; j<30 ; j++){
            switch(mapa[i][j]){
                case 'Z':
                    DrawRectangle(posX, posY+CABECALHO, COMP_LINHA, COMP_COLUNA, BROWN);
                    break;
                case 'S':
                case 'H':
                case 'D':
                    DrawRectangle(posX, posY+CABECALHO, COMP_LINHA, COMP_COLUNA, ORANGE);
                    break;
                case 'F':
                    DrawRectangle(posX, posY+CABECALHO, COMP_LINHA, COMP_COLUNA, RED);
            }
            posX+=COMP_LINHA;
        }
        posY+=COMP_COLUNA;
    }
}
