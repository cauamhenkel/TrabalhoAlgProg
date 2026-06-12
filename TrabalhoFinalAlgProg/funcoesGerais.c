#include "funcoesGerais.h"

void lePlacar(TIPO_PLACAR placar[TAM_PLACAR]){
    FILE *leitura;
    // Se não leu nada, termina a função
    if ((leitura = fopen("placar.bin", "rb"))==NULL){
        return;
    }
    // Se leu alguma coisa, salva o que conseguiu ler no vetor.
    // Caso não tenha dados suficientes para preencher todo o vetor, o que não recebeu nada continua vazio (com zeros)
    fread(placar, sizeof(TIPO_PLACAR), TAM_PLACAR, leitura);

    fclose(leitura);
}

void salvaPlacar(TIPO_PLACAR placar[TAM_PLACAR]){
    FILE *escrita;
    // Abre o arquivo como escrita binária, destrói o placar antigo se ja existia
    escrita = fopen("placar.bin", "wb");
    // Escreve todo o placar no arquivo binário
    fwrite(placar, sizeof(TIPO_PLACAR), TAM_PLACAR, escrita);

    fclose (escrita);
}

void desenhaBotao(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, const char texto[20]){
/* Essa função desenha um botão com as bordas arredondadas e um texto no centro */
    // Salva as dimensões e posição do retângulo de entrada
    Rectangle retanguloBase = {posX, posY, largura, altura};
    // Cria um retangulo com proporções ajustadas para ser uma borda
    Rectangle retanguloBorda = {posX-(COMP_LINHA/4), posY-(COMP_COLUNA/4), largura+(COMP_LINHA/2), altura+(COMP_COLUNA/2)};
    // Desenha a borda do botão
    DrawRectangleRounded(retanguloBorda, 0.4, 6, corBorda);
    // Desenha o interior do botão
    DrawRectangleRounded(retanguloBase, 0.3, 6, corDentro);
    // Faz o texto dentro do botão
    DrawText(texto,(LARGURA/2)-(MeasureText(texto, FONTE_BOTOES)/2), posY+(altura/2)-(FONTE_BOTOES/2), FONTE_BOTOES, RED);
}

void desenhaBotaoRanking(int posX, int posY, int largura, int altura, Color corDentro, Color corBorda, TIPO_PLACAR placar){
/* Essa função desenha os retângulos do ranking com nome e pontos dentro, dando cor dependendo do modo vencido */
    // Salva as dimensões e posição do retângulo de entrada
    Rectangle retanguloBase = {posX, posY, largura, altura};
    // Cria um retangulo com proporções ajustadas para ser uma borda
    Rectangle retanguloBorda = {posX-5, posY-5, largura+10, altura+10};
    // Desenha a borda do botão
    DrawRectangleRounded(retanguloBorda, 0.4, 6, corBorda);
    // Desenha o interior do botão
    DrawRectangleRounded(retanguloBase, 0.3, 6, corDentro);
    // Se a posição no placar está vazia (ainda não encheu o placar de pessoas) coloca uma escrita genérica
    if (placar.nome[0]=='\0'){
        DrawText(TextFormat("Nome: -------------------"), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/5), FONTE_RANKING, RED);
        DrawText(TextFormat("Pontos: XxxxX"), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/2)+FONTE_RANKING, FONTE_RANKING, RED);
    }
    // Se houver algum nome escreve os nomes no ranking, com a cor da fonte dependendo do modo que o player venceu
    else{
        DrawText(TextFormat("Nome: %s", placar.nome), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/5), FONTE_RANKING, (placar.modoVitoria==PACIFISTA) ? DARKGREEN : (placar.modoVitoria==GENOCIDA) ? BLUE : RED);
        DrawText(TextFormat("Pontos: %d",placar.pontos), posX+(COMP_LINHA/5), posY+(COMP_COLUNA/2)+FONTE_RANKING, FONTE_RANKING, (placar.modoVitoria==PACIFISTA) ? DARKGREEN : (placar.modoVitoria==GENOCIDA) ? BLUE : RED);
    }
}

void desenhaTextoMenu(EstadoMenu opcaoMenu){
/* Essa função desenha todo o conteúdo presente no menu do jogo */
    // Algumas constantes para posicionar os botões corretamente na tela
    int posBotao1 = COMP_COLUNA * 10,
        posBotao2 = posBotao1 + (COMP_COLUNA * 5),
        posBotao3 = posBotao2 + (COMP_COLUNA * 5);
    // Desenha o titulo
    DrawText("Mario Games", (LARGURA/2) - (MeasureText("Mario Games", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    // Desenha os botões, sendo que eles mudam de cor dependendo da opção do menu selecionada
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao1, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_JOGAR) ? GOLD : ORANGE, RED, "Jogar");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao2, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_RANKING) ? GOLD : ORANGE, RED, "Ranking");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao3, LARG_BOTOES, ALT_BOTOES, (opcaoMenu==MENU_SAIR) ? GOLD : ORANGE, RED, "Sair");
}

void desenhaTextoRanking(TIPO_PLACAR placar[TAM_PLACAR]){
/* Essa função desenha todo o conteúdo presente no menu de ranking jogo */
    // Título do menu ramking
    DrawText("Ranking", (LARGURA/2) - (MeasureText("Ranking", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    // Desenha todos os botões na esquerda
    for (int i=0 ; i<(TAM_PLACAR/2) ; i++){
        desenhaBotaoRanking((LARGURA/2)-LARG_BOTOES_RANKING-(COMP_LINHA/2), ALT_BOTOES*(i+2)*1.2, LARG_BOTOES_RANKING, ALT_BOTOES_RANKING, GOLD, RED, placar[i]);
    }
    // Desenha todos os botões na direita
    for (int i=0 ; i<(TAM_PLACAR/2) ; i++){
        desenhaBotaoRanking((LARGURA/2)+(COMP_LINHA/2), ALT_BOTOES*(i+2)*1.2, LARG_BOTOES_RANKING, ALT_BOTOES_RANKING, GOLD, RED, placar[i+TAM_PLACAR/2]);
    }
    // Desenha um botão em baixo para voltar para o menu
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), COMP_COLUNA * 25, LARG_BOTOES, ALT_BOTOES, GOLD, RED, "Voltar ao menu");
}

void desenhaTextoPause(EstadoPausado opcaoPause){
/* Essa função desenha todo o conteúdo presente no menu de pause jogo */
    // Algumas constantes para posicionar os botões corretamente na tela
    int posBotao1 = COMP_COLUNA * 10,
        posBotao2 = posBotao1 + (COMP_COLUNA * 5),
        posBotao3 = posBotao2 + (COMP_COLUNA * 5);

    // Desenha o titulo
    DrawText("Jogo pausado", (LARGURA/2) - (MeasureText("Jogo pausado", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    // Desenha os botões, sendo que eles mudam de cor dependendo da opção do menu selecionada
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao1, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_CONTINUAR) ? GOLD : ORANGE, RED, "Continuar");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao2, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_VOLTAR_AO_MENU) ? GOLD : ORANGE, RED, "Voltar ao menu");
    desenhaBotao((LARGURA/2)-(LARG_BOTOES/2), posBotao3, LARG_BOTOES, ALT_BOTOES, (opcaoPause==PAUSE_SAIR) ? GOLD : ORANGE, RED, "Sair");
}

void desenhaTextoVitoria(int pontos, char nomeTemp[TAM_NOME_RANKING+1], int *piscando, ModoVitoria modoVitoria){
/* Essa função desenha todo o conteúdo presente no menu de vitória jogo */
    // Desenha os textos de vitória e traz uma mensagem adicional se o jogador terminar sem matar nenhum monstro
    DrawText("Voce venceu!", (LARGURA/2) - (MeasureText("Voce venceu!", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, (modoVitoria==GENOCIDA) ? RED : GREEN);
    if (modoVitoria==PACIFISTA){
        DrawText("No modo pacifista!", (LARGURA/2) - (MeasureText("No modo pacifista!", FONTE_GERAL)/2), CABECALHO*2.5, FONTE_GERAL, GREEN);
    }
    if (modoVitoria==GENOCIDA){
        DrawText("No modo genocida!", (LARGURA/2) - (MeasureText("No modo genocida!", FONTE_GERAL)/2), CABECALHO*2.5, FONTE_GERAL, RED);
    }
    // A altura do texto dos pontos depende se há o texto sobre o modo pacifista/genocida ou não
    DrawText(TextFormat("Voce fez %d pontos",pontos), (LARGURA/2) - (MeasureText("Voce fez XxxX pontos", FONTE_CABECALHO)/2), (modoVitoria==NORMAL) ? CABECALHO*3 : CABECALHO*4.5, FONTE_CABECALHO, (modoVitoria==GENOCIDA) ? RED : GREEN);
    DrawText("Digite seu nome:", (LARGURA/2) - (MeasureText("Digite seu nome:", FONTE_CABECALHO)/2), ALTURA/2, FONTE_CABECALHO, WHITE);
    // Desenha um retângulo para ser o fundo de onde será escrito o nome
    DrawRectangle(COMP_LINHA*2, (ALTURA+CABECALHO*2)/2, LARGURA-4*COMP_LINHA, COMP_COLUNA*2, WHITE);
    // Escreve o nome que está sendo escrito em tempo real pelo jogador
    DrawText(TextFormat("%s",nomeTemp), COMP_LINHA*2.5, ((ALTURA+CABECALHO*2)/2)+COMP_COLUNA-(FONTE_CABECALHO/2), FONTE_CABECALHO, BLACK);
    // Coloca uma barrinha piscando no final para indicar onde está escrevendo
    DrawText(TextFormat(" |"), MeasureText(TextFormat("%s", nomeTemp), FONTE_CABECALHO)+COMP_LINHA*2.5, ((ALTURA+CABECALHO*2)/2)+COMP_COLUNA-(FONTE_CABECALHO/2), FONTE_CABECALHO, (*piscando % (FPS*2)) > (FPS) ? WHITE : BLACK);
    (*piscando)++;
}

void desenhaTextoDerrota(void){
/* Essa função desenha todo o conteúdo presente no menu de derrota jogo */
    // Desenha os textos na tela
    DrawText("Voce morreu!", (LARGURA/2) - (MeasureText("Voce morreu!", FONTE_GERAL)/2), CABECALHO, FONTE_GERAL, RED);
    DrawText("Pressione ENTER para voltar ao menu", (LARGURA/2) - (MeasureText("Pressione ENTER para voltar ao menu", FONTE_CABECALHO)/2), (ALTURA+CABECALHO)/2, FONTE_CABECALHO, WHITE);
}

void criaMapa(char mapa[TILES][TILES], int fase){
/* Essa função recebe um número de fase e transforma o arquivo .txt formatado dessa fase para ser um mapa em uma matriz 30 por 30 */
    FILE *leitura;
    // Variável para salvar momentaneamente o nome do arquivo
    char nomeArquivo[50];
    // Variável para salvar momentaneamente o caractere lido do arquivo
    char ch;
    // Salva o nome do arquivo que será aberto com base no número da fase de entrada
    sprintf(nomeArquivo, "assets/levels/mapa%d.txt", fase);

    leitura = fopen(nomeArquivo, "r");
    // Itera um número de vezes igual TILES x TILES
    for (int i=0 ; i<TILES ; i++){
        for (int j=0 ; j<TILES ; j++){
            // Lê o caractere do arquivo
            ch=fgetc(leitura);
            // Se o caractere lido for algum de formatação (como enter), lê o próximo e descarta o atual
            while (ch=='\n' || ch=='\r'){
                ch=fgetc(leitura);
            }
            mapa[i][j]=ch;
        }
    }
    fclose(leitura);
}

void corrigeMapa(char mapa[TILES][TILES]){
/* Essa função lê todo o mapa e substitui a letra que fica logo abaixo do D no grid por X */
    for(int i=0 ; i<TILES ; i++){
        for(int j=0 ; j<TILES ; j++){
            if (mapa[i][j]=='D')
                mapa[i+1][j]='X';
        }
    }
}

void reduzPontos(int *pontos, int *framesReduzirPontos){
/* Essa função diminui os pontos toda vez que se passa uma quantidade de frames especificada pelas constantes */
    if (*framesReduzirPontos > FRAMES_PARA_REDUZIR_PONTOS){
        // Se tiver mais que 100 pontos (o mínimo possível) reduz pela quantidade estipulada e reinicia o contador
        if (*pontos>100){
            *pontos-=QTD_REDUZIR_PONTOS;
            *framesReduzirPontos=0;
        }
        // Se tiver menos que 100 pontos corrige
        if (*pontos<100){
            *pontos=100;
        }
    }
    (*framesReduzirPontos)++;
}

void reiniciaNome(char nomeTemp[TAM_NOME_RANKING]){
/* Essa função reinicia o nome temporário para impedir que se salve lixo entre uma vitória e outra */
    for (int i=0 ; i<(TAM_NOME_RANKING) ; i++){
        nomeTemp[i]=0;
    }
}

void colocaNoPlacar(TIPO_PLACAR placar[TAM_PLACAR], char nomeTemp[TAM_NOME_RANKING], int pontos, ModoVitoria modoVitoria){
/* Essa função recebe o placar, o nome da pessoa e os pontos dela e coloca no placar se ela tiver os requisitos necessários */
    int nomeRepetido=0;
    // Analisa se já existe o nome no placar e se existir, substitui os pontos existentes se os pontos atuais forem maiores
    // Senão, não adiciona nada no placar
    for (int i=0 ; i<TAM_PLACAR ; i++){
        if (strcmp(placar[i].nome,nomeTemp)==0){
            if (placar[i].pontos<pontos){
                placar[i].pontos=pontos;
                placar[i].modoVitoria=modoVitoria;
            }
            nomeRepetido=1;
        }
    }
    // Se o nome é novo, analisa a última posição do placar e adiciona se os pontos atuais forem maiores do que os que ja estavam salvos
    if (!nomeRepetido){
        if (pontos>placar[TAM_PLACAR-1].pontos){
            strcpy(placar[TAM_PLACAR-1].nome, nomeTemp);
            placar[TAM_PLACAR-1].pontos=pontos;
            placar[TAM_PLACAR-1].modoVitoria=modoVitoria;
        }
    }
}

void ordenarPlacar(TIPO_PLACAR placar[TAM_PLACAR]){
/* Essa função ordena o placar de entrada usando os pontos como referência */
    TIPO_PLACAR temp;
    // Ordenação por selection sort
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
/* Essa função testa se foi alcançada a vitória, dando a vitória sempre que não houver mais nenhum mapa a ser lido */
    FILE *leitura;
    // Variável para salvar momentaneamente o nome do arquivo
    char nomeArquivo[50];
    // Salva o nome do arquivo que será aberto com base no número da fase de entrada
    sprintf(nomeArquivo, "assets/levels/mapa%d.txt", fase);
    // Abre o arquivo de entrada no ponteiro e se a leitura der errado quer dizer que já passou todos os mapas
    if ((leitura = fopen(nomeArquivo, "r")) == NULL){
        return 1;
    }
    // Se conseguir abrir quer dizer que ainda há fases a serem jogadas
    else{
        fclose(leitura);
        return 0;
    }
}
