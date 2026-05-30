#include "funcoesGerais.h"

void lePlacar(TIPO_PLACAR placar[TAM_PLACAR]){
    FILE *leitura;

    if ((leitura = fopen("placar.bin", "rb"))==NULL){
        return;
    }
    fread(placar, sizeof(TIPO_PLACAR), TAM_PLACAR, leitura);

    fclose(leitura);
}

void salvaPlacar(TIPO_PLACAR placar[TAM_PLACAR]){
    FILE *escrita;

    escrita = fopen("placar.bin", "wb");

    fwrite(placar, sizeof(TIPO_PLACAR), TAM_PLACAR, escrita);

    fclose (escrita);
}

void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]){
/* Essa função desenha um botão com as bordas arredondadas e um texto no centro */
    Rectangle retanguloBase = {posX, posY, largura, altura};             // Salva as dimensões e posição do retangulo de entrada
    Rectangle retanguloBorda = {posX-5, posY-5, largura+10, altura+10};  // Cria um retangulo com proporções ajustadas
    DrawRectangleRounded(retanguloBorda, 0.4, 6, corBorda);              // Desenha a borda do retangulo
    DrawRectangleRounded(retanguloBase, 0.3, 6, corDentro);              // Desenha o retangulo
    DrawText(texto,(LARGURA/2)-(MeasureText(texto, FONTE_BOTOES)/2), posY+(altura/2)-(FONTE_BOTOES/2), FONTE_BOTOES, RED); // Faz o texto dentro do botão
}

void desenhaBotaoRanking(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, TIPO_PLACAR placar){
    Rectangle retanguloBase = {posX, posY, largura, altura};             // Salva as dimensões e posição do retangulo de entrada
    Rectangle retanguloBorda = {posX-5, posY-5, largura+10, altura+10};  // Cria um retangulo com proporções ajustadas
    DrawRectangleRounded(retanguloBorda, 0.4, 6, corBorda);              // Desenha a borda do retangulo
    DrawRectangleRounded(retanguloBase, 0.3, 6, corDentro);              // Desenha o retangulo
    if (placar.nome[0]=='\0'){
        DrawText(TextFormat("Nome: -------------------"), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/5), FONTE_RANKING, RED);
        DrawText(TextFormat("Pontos: XxxxX"), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/2)+FONTE_RANKING, FONTE_RANKING, RED);
    }
    else{
        DrawText(TextFormat("Nome: %s", placar.nome), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/5), FONTE_RANKING, RED);
        DrawText(TextFormat("Pontos: %d",placar.pontos), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/2)+FONTE_RANKING, FONTE_RANKING, RED);
    }
}

void desenhaTextoMenu(EstadoMenu opcaoMenu){
    int posBotao1 = COMP_COLUNA * 10,
        posBotao2 = posBotao1 + (COMP_COLUNA * 5),
        posBotao3 = posBotao2 + (COMP_COLUNA * 5);

    DrawText("Mario Games", (LARGURA/2) - (MeasureText("Mario Games", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao1, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_JOGAR) ? GOLD : ORANGE, RED, "Jogar");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao2, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_RANKING) ? GOLD : ORANGE, RED, "Ranking");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao3, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_SAIR) ? GOLD : ORANGE, RED, "Sair");
}

void desenhaTextoRanking(TIPO_PLACAR placar[TAM_PLACAR]){
    DrawText("Ranking", (LARGURA/2) - (MeasureText("Ranking", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    for (int i=0 ; i<(TAM_PLACAR/2) ; i++){
        desenhaBotaoRanking((LARGURA/2)-LARG_BOTOES_RANKING-(COMP_LINHA/2), ALT_BOTOES*(i+2)*1.2, LARG_BOTOES_RANKING, ALT_BOTOES_RANKING, GOLD, RED, placar[i]);
    }
    for (int i=0 ; i<(TAM_PLACAR/2) ; i++){
        desenhaBotaoRanking((LARGURA/2)+(COMP_LINHA/2), ALT_BOTOES*(i+2)*1.2, LARG_BOTOES_RANKING, ALT_BOTOES_RANKING, GOLD, RED, placar[i+TAM_PLACAR/2]);
    }
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), COMP_COLUNA * 25, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Voltar ao menu");
}

void desenhaTextoPause(EstadoPausado opcaoPause){
    int posBotao1 = COMP_COLUNA * 10,
        posBotao2 = posBotao1 + (COMP_COLUNA * 5),
        posBotao3 = posBotao2 + (COMP_COLUNA * 5);

    DrawText("Jogo pausado", (LARGURA/2) - (MeasureText("Jogo pausado", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao1, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_CONTINUAR) ? GOLD : ORANGE, RED, "Continuar");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao2, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_VOLTAR_AO_MENU) ? GOLD : ORANGE, RED, "Voltar ao menu");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao3, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_SAIR) ? GOLD : ORANGE, RED, "Sair");
}

void desenhaTextoVitoria(int pontos, char nomeTemp[TAM_NOME_RANKING+1]){
    DrawText("Voce venceu!", (LARGURA/2) - (MeasureText("Voce venceu!", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, GREEN);
    DrawText(TextFormat("Voce fez %d pontos",pontos), (LARGURA/2) - (MeasureText("Voce fez XxxX pontos", FONTE_CABECALHO)/2), CABECALHO*3, FONTE_CABECALHO, GREEN);
    DrawText("Digite seu nome:", (LARGURA/2) - (MeasureText("Digite seu nome:", FONTE_CABECALHO)/2), ALTURA/2, FONTE_CABECALHO, WHITE);
    DrawRectangle(COMP_LINHA*2, (ALTURA+CABECALHO*2)/2, LARGURA-4*COMP_LINHA, COMP_COLUNA*2, WHITE);
    DrawText(TextFormat("%s",nomeTemp), COMP_LINHA*2.5, ((ALTURA+CABECALHO*2)/2)+COMP_COLUNA-(FONTE_CABECALHO/2), FONTE_CABECALHO, BLACK);
    DrawText(TextFormat("|",nomeTemp), (COMP_LINHA*2.5)+MeasureText(TextFormat("%s",nomeTemp), FONTE_CABECALHO), ((ALTURA+CABECALHO*2)/2)+COMP_COLUNA-(FONTE_CABECALHO/2), FONTE_CABECALHO, BLACK);
}

void desenhaTextoDerrota(void){
    DrawText("Voce morreu!", (LARGURA/2) - (MeasureText("Voce morreu!", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    DrawText("Pressione ENTER para voltar ao menu", (LARGURA/2) - (MeasureText("Pressione ENTER para voltar ao menu", FONTE_CABECALHO)/2), (ALTURA+CABECALHO)/2, FONTE_CABECALHO, WHITE);
}

void criaMapa(char mapa[TILES][TILES], int fase){
/* Essa função recebe um número de fase e transforma o arquivo .txt formatado dessa fase para ser um mapa em uma matriz 30 por 30 */
/* Se passar da ultima fase (não tem mais nenhum arquivo para ler) dá a vitória */
    FILE *leitura;                                    // Ponteiro usado para ler o arquivo
    char nomeArquivo[50];                             // Variável para salvar momentaneamente o nome do arquivo
    char ch;                                          // Variável para salvar momentaneamente o caractere lido do arquivo

    sprintf(nomeArquivo, "assets/levels/mapa%d.txt", fase);         // Salva o nome do arquivo que será aberto com base no número da fase de entrada

    leitura = fopen(nomeArquivo, "r");

    for (int i=0 ; i<TILES ; i++){
        for (int j=0 ; j<TILES ; j++){
            ch=fgetc(leitura);                    // Lê o caractere do arquivo
            while (ch=='\n' || ch=='\r')          // Se o caractere lido for algum de formatação (como enter)
                ch=fgetc(leitura);                // Ignora e pega o próximo caractere, fica em loop até achar um caractere normal
            mapa[i][j]=ch;                        // Salva o caractere na matriz
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

void reduzPontos(int *pontos, float *tempoAtual, float *tempoAnterior){
    *tempoAtual=GetTime();
    if ((*tempoAtual - *tempoAnterior) > TEMP_PARA_REDUZIR_PONTOS){
        if (*pontos>100){
            *pontos-=QTD_REDUZIR_PONTOS;
            *tempoAnterior=*tempoAtual;
        }
        if (*pontos<100){
            *pontos=100;
        }
    }
}

void reiniciaNome(char nomeTemp[TAM_NOME_RANKING]){
    for (int i=0 ; i<(TAM_NOME_RANKING) ; i++){
        nomeTemp[i]=0;
    }
}

void colocaNoPlacar(TIPO_PLACAR placar[TAM_PLACAR], char nomeTemp[TAM_NOME_RANKING], int pontos){
    int nomeRepetido=0;
    for (int i=0 ; i<TAM_PLACAR ; i++){
        if (strcmp(placar[i].nome,nomeTemp)==0){
            if (placar[i].pontos<pontos){
                placar[i].pontos=pontos;
            }
            nomeRepetido=1;
        }
    }
    if (!nomeRepetido){
        if (pontos>placar[TAM_PLACAR-1].pontos){
            strcpy(placar[TAM_PLACAR-1].nome, nomeTemp);
            placar[TAM_PLACAR-1].pontos=pontos;
        }
    }
}

void ordenarPlacar(TIPO_PLACAR placar[TAM_PLACAR]){
    TIPO_PLACAR temp;
    for (int i=0 ; i<TAM_PLACAR-1 ; i++){
        for (int j=i+1 ; j<TAM_PLACAR ; j++){
            if (placar[i].pontos < placar[j].pontos){
                temp=placar[i];
                placar[i]=placar[j];
                placar[j]=temp;
            }
        }
    }
}

int vitoria(char mapa[TILES][TILES], int fase){
    FILE *leitura;                                    // Ponteiro usado para ler o arquivo
    char nomeArquivo[50];                             // Variável para salvar momentaneamente o nome do arquivo

    sprintf(nomeArquivo, "assets/levels/mapa%d.txt", fase);         // Salva o nome do arquivo que será aberto com base no número da fase de entrada

    if ((leitura = fopen(nomeArquivo, "r")) == NULL){ // Abre o arquivo de entrada no ponteiro e se a leitura der errado (passou todos os mapas)
        fclose(leitura);
        return 1;
    }
    else{
        fclose(leitura);
        return 0;
    }
}
