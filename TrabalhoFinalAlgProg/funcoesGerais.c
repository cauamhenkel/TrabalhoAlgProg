#include "funcoesGerais.h"

void reiniciaFase(int *iniciouMapa, int *iniciouPlayer, int *iniciouMonstros){
/* Essa função reinicia as flags para iniciar o jogo */
    (*iniciouMapa)=0;
    (*iniciouPlayer)=0;
    (*iniciouMonstros)=0;
}

void criaMapa(char mapa[30][30], int fase, EstadoJogo *estado){
/* Essa função recebe um número de fase e transforma o arquivo .txt formatado dessa fase para ser um mapa em uma matriz 30 por 30 */
/* Se passar da ultima fase (não tem mais nenhum arquivo para ler) dá a vitória */
    FILE *leitura;                           // Ponteiro usado para ler o arquivo
    char nomeArquivo[12];
    char ch;                                 // Variável para salvar momentaneamente o caractere lido do arquivo

    sprintf(nomeArquivo, "mapa%d.txt", fase); // Salva o nome do arquivo que será aberto com base no número da fase de entrada

    if ((leitura = fopen(nomeArquivo, "r")) == NULL){ // Abre o arquivo de entrada no ponteiro e se a leitura der errado
        (*estado)=VITORIA;                            // Determina vitória (passou da última fase)
    }
    else{
        for (int i=0 ; i<30 ; i++){
            for (int j=0 ; j<30 ; j++){
                ch=fgetc(leitura);               // Lê o caractere do arquivo
                while (ch=='\n' || ch=='\r')     // Se o caractere lido for algum de formatação (como enter)
                    ch=fgetc(leitura);           // Ignora e pega o próximo caractere, fica em loop até achar um caractere normal
                mapa[i][j]=ch;                   // Salva o caractere na matriz
            }
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

void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]){
    Rectangle retanguloBase = {posX, posY, largura, altura};
    Rectangle retanguloBorda = {posX-5, posY-5, largura+10, altura+10};
    DrawRectangleRounded(retanguloBorda, 0.4, 6, corBorda);
    DrawRectangleRounded(retanguloBase, 0.3, 6, corDentro);
    DrawText(texto,(LARGURA/2)-(MeasureText(texto, FONTE_BOTOES)/2), posY+(altura/2)-(FONTE_BOTOES/2), FONTE_BOTOES, RED);
}

void exibeFase(int fase){
/* Essa função exibe no canto da tela a fase atual */
    DrawText(TextFormat("Fase atual: %d", fase+1), 450, 10, 20, RED);
}
