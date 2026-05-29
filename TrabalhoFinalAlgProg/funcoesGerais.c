#include "funcoesGerais.h"

void criaMapa(char mapa[TILES][TILES], int fase, EstadoJogo *estado){
/* Essa função recebe um número de fase e transforma o arquivo .txt formatado dessa fase para ser um mapa em uma matriz 30 por 30 */
/* Se passar da ultima fase (não tem mais nenhum arquivo para ler) dá a vitória */
    FILE *leitura;                                    // Ponteiro usado para ler o arquivo
    char nomeArquivo[50];                             // Variável para salvar momentaneamente o nome do arquivo
    char ch;                                          // Variável para salvar momentaneamente o caractere lido do arquivo

    sprintf(nomeArquivo, "assets/levels/mapa%d.txt", fase);         // Salva o nome do arquivo que será aberto com base no número da fase de entrada

    if ((leitura = fopen(nomeArquivo, "r")) == NULL){ // Abre o arquivo de entrada no ponteiro e se a leitura der errado (passou todos os mapas)
        (*estado)=VITORIA;                            // Determina vitória (passou da última fase)
    }
    else{
        for (int i=0 ; i<TILES ; i++){
            for (int j=0 ; j<TILES ; j++){
                ch=fgetc(leitura);                    // Lê o caractere do arquivo
                while (ch=='\n' || ch=='\r')          // Se o caractere lido for algum de formatação (como enter)
                    ch=fgetc(leitura);                // Ignora e pega o próximo caractere, fica em loop até achar um caractere normal
                mapa[i][j]=ch;                        // Salva o caractere na matriz
            }
        }
    }
    fclose(leitura);                                  // Fecha o arquivo
}

void corrigeMapa(char mapa[TILES][TILES]){
    for(int i=0 ; i<TILES ; i++){
        for(int j=0 ; j<TILES ; j++){
            if (mapa[i][j]=='D')
                mapa[i+1][j]='X';
        }
    }
}

void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]){
/* Essa função desenha um botão com as bordas arredondadas e um texto no centro */
    Rectangle retanguloBase = {posX, posY, largura, altura};             // Salva as dimensões e posição do retangulo de entrada
    Rectangle retanguloBorda = {posX-5, posY-5, largura+10, altura+10};  // Cria um retangulo com proporções ajustadas
    DrawRectangleRounded(retanguloBorda, 0.4, 6, corBorda);              // Desenha a borda do retangulo
    DrawRectangleRounded(retanguloBase, 0.3, 6, corDentro);              // Desenha o retangulo
    DrawText(texto,(LARGURA/2)-(MeasureText(texto, FONTE_BOTOES)/2), posY+(altura/2)-(FONTE_BOTOES/2), FONTE_BOTOES, RED); // Faz o texto dentro do botão
}

void reduzPontos(int *pontos, float *tempoAtual, float *tempoAnterior){
    *tempoAtual=GetTime();
    if ((*tempoAtual - *tempoAnterior) > TEMP_PARA_REDUZIR_PONTOS){
        *pontos-=QTD_REDUZIR_PONTOS;
        *tempoAnterior=*tempoAtual;
    }
}
