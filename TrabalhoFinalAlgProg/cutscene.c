#include "raylib.h"
#include "custcene.h"
#include <string.h>

/*
#define TILES 30
#define COMP_LINHA 28
#define COMP_COLUNA 28
#define LARGURA (COMP_LINHA*TILES)
#define CABECALHO (COMP_COLUNA*2)
#define ALTURA (COMP_COLUNA*TILES)
#define FONTE_GERAL (COMP_LINHA*2)
#define FPS 60 */

const int SCREEN_SIZE = COMP_LINHA*20;
const int SCREEN_POS_X = LARGURA/2 - SCREEN_SIZE/2;
const int SCREEN_POS_Y = CABECALHO*2;

const int TEXT_SIZE = COMP_LINHA*1.5;
const int TEXT_POS_X = SCREEN_POS_X - COMP_LINHA*2;
const int TEXT_DIST_Y = TEXT_SIZE*1.4;
const int TEXT_POS_Y0 = (ALTURA+CABECALHO)*6.7/10;
const int TEXT_POS_Y1 = TEXT_POS_Y0 + TEXT_DIST_Y;
const int TEXT_POS_Y2 = TEXT_POS_Y1 + TEXT_DIST_Y;
const int TEXT_POS_Y3 = TEXT_POS_Y2 + TEXT_DIST_Y;

void cutscene(void) {
    TextBox dialogo = {
        .linhas = {"", "", "", ""},
        .mensagens = {"Macondo, Colombia, 1970.",
            "Filho bastardo do prefeito, nasce:",
            "El Cowboy Emiliano.",
            ""},
        .posY = {TEXT_POS_Y0, TEXT_POS_Y1, TEXT_POS_Y2, TEXT_POS_Y3},
        .timer = 0.0,
        .iterador = 0,
        .linhaAtual = 0,
    };

    int frameAtual = 1;

    int sair_custcene = 0;

    InitWindow(LARGURA, ALTURA+CABECALHO, "Cutscene");
    SetTargetFPS(FPS);

    while (!WindowShouldClose() && sair_custcene == 0) {


        BeginDrawing();
            ClearBackground(BLACK);

            DrawRectangle(SCREEN_POS_X, SCREEN_POS_Y, SCREEN_SIZE, SCREEN_SIZE*3/4, GRAY);

            writeLine(&dialogo, &frameAtual);

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER))
            sair_custcene = 67;
    }

    CloseWindow();
}

void writeLine(TextBox *tb, int *frame) {
    tb->timer += GetFrameTime();

    if (IsKeyPressed(KEY_K)) {
        for (int k=0; k<TB_LINES; k++)
            strcpy(tb->linhas[k], "");
        (*frame)++;
        tb->linhaAtual = 0;
        tb->timer = 0;
        tb->iterador = 0;
        updateLine(tb, *frame);
    }

    if (tb->linhaAtual < TB_LINES && tb->timer >= 1.0/10.0 && tb->mensagens[tb->linhaAtual][tb->iterador] != '\0') {
        tb->timer = 0.0;
        tb->linhas[tb->linhaAtual][tb->iterador] = tb->mensagens[tb->linhaAtual][tb->iterador];
        tb->iterador++;
        tb->linhas[tb->linhaAtual][tb->iterador] = '\0';

        if (tb->mensagens[tb->linhaAtual][tb->iterador] == '\0') {
            tb->linhaAtual++;
            tb->iterador=0;
        }
    }

    if (tb->linhaAtual == TB_LINES){
        if (IsKeyPressed(KEY_K)) {
            for (int k=0; k<TB_LINES; k++)
                strcpy(tb->linhas[k], "");
            (*frame)++;
            tb->linhaAtual = 0;
            updateLine(tb, *frame);
        }
    }

    for (int k=0; k< TB_LINES; k++) {
        DrawText(tb->linhas[k], TEXT_POS_X, tb->posY[k], TEXT_SIZE, WHITE);
    }
}

void updateLine(TextBox *tb, int frame) {
    switch (frame) {
    case 2:
        strcpy(tb->mensagens[0], "Seus hobbies de infancia eram");
        strcpy(tb->mensagens[1], "atirar com revolver, subir");
        strcpy(tb->mensagens[2], "escadas e procurar gemas.");
        break;

    case 3:
        strcpy(tb->mensagens[0], "Aos 28 anos de idade,");
        strcpy(tb->mensagens[1], "casou-se com Oriana, o amor");
        strcpy(tb->mensagens[2], "de sua vida.");
        break;

    case 4:
        strcpy(tb->mensagens[0], "O casamento durou 25 anos,");
        strcpy(tb->mensagens[1], "tiveram 3 filhos, mas acabaram");
        strcpy(tb->mensagens[2], "se divorciando.");
        break;

    case 5:
        strcpy(tb->mensagens[0], "Depois disso tudo, o que");
        strcpy(tb->mensagens[1], "sobrou para El Nuestro Cowboy");
        strcpy(tb->mensagens[2], "foram seus antigos talentos.");
        break;

    case 6:
        strcpy(tb->mensagens[0], "Atirar com o revolver,");
        strcpy(tb->mensagens[1], "subir escadas e");
        strcpy(tb->mensagens[2], "procurar gemas preciosas.");
        break;

    case 7:
        strcpy(tb->mensagens[0], "Nao importa quantos cavalos");
        strcpy(tb->mensagens[1], "zumbi ele tenha que matar (ou");
        strcpy(tb->mensagens[2], "ignorar) pelo caminho.");
        break;
    }
}
