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

    Rectangle fonte = {0,0,48,36};
    Rectangle destino = {SCREEN_POS_X, SCREEN_POS_Y, SCREEN_SIZE, SCREEN_SIZE*3/4};
    int sair_custcene = 0;

    InitWindow(LARGURA, ALTURA+CABECALHO, "Cutscene");
    SetTargetFPS(FPS);

    Texture2D cenas[5];
    cenas[0] = LoadTexture("cutscene/cutscene1.png");
    cenas[1] = LoadTexture("cutscene/cutscene2.png");
    cenas[2] = LoadTexture("cutscene/cutscene3.png");
    cenas[3] = LoadTexture("cutscene/cutscene4.png");
    cenas[4] = LoadTexture("cutscene/cutscene5.png");

    int iteraCena = 0;

    InitAudioDevice();
    Music undertale = LoadMusicStream("assets/audio/cutscene_undertale.mp3");
    PlayMusicStream(undertale);

    while (!WindowShouldClose() && sair_custcene == 0) {
        Texture2D cenaAtual = cenas[iteraCena];

        BeginDrawing();
            ClearBackground(BLACK);

            DrawTexturePro(cenaAtual, fonte, destino, (Vector2){0,0}, 0.0f, WHITE);

            writeLine(&dialogo, &frameAtual, &iteraCena);

        EndDrawing();

        UpdateMusicStream(undertale);

        if (IsKeyPressed(KEY_ENTER))
            sair_custcene = 67;
    }

    UnloadMusicStream(undertale);
    CloseAudioDevice();

    UnloadTexture(cenas[0]);
    UnloadTexture(cenas[1]);
    UnloadTexture(cenas[2]);
    UnloadTexture(cenas[3]);
    UnloadTexture(cenas[4]);

    CloseWindow();
}

void writeLine(TextBox *tb, int *frame, int *iteraCena) {
    tb->timer += GetFrameTime();

    if (IsKeyPressed(KEY_K)) {
        for (int k=0; k<TB_LINES; k++)
            strcpy(tb->linhas[k], "");
        (*frame)++;
        tb->linhaAtual = 0;
        tb->timer = 0;
        tb->iterador = 0;
        updateLine(tb, *frame, iteraCena);
    }

    if (tb->linhaAtual < TB_LINES && tb->timer >= 1.0/10.0 && tb->mensagens[tb->linhaAtual][tb->iterador] != '\0') {
        tb->timer = 0.0;
        tb->linhas[tb->linhaAtual][tb->iterador] = tb->mensagens[tb->linhaAtual][tb->iterador];
        tb->iterador++;
        tb->linhas[tb->linhaAtual][tb->iterador] = '\0';

        if (tb->mensagens[tb->linhaAtual][tb->iterador] == '\0') {
            if (*frame == 4 && tb->linhaAtual == 1){
                (*iteraCena)++;
                tb->linhaAtual++;
                tb->iterador=0; }
            else {
            tb->linhaAtual++;
            tb->iterador=0; }
        }
    }

    if (tb->linhaAtual == TB_LINES){
        if (IsKeyPressed(KEY_K)) {
            for (int k=0; k<TB_LINES; k++)
                strcpy(tb->linhas[k], "");
            (*frame)++;
            tb->linhaAtual = 0;
            updateLine(tb, *frame, iteraCena);
        }
    }

    for (int k=0; k< TB_LINES; k++) {
        DrawText(tb->linhas[k], TEXT_POS_X, tb->posY[k], TEXT_SIZE, WHITE);
    }
}

void updateLine(TextBox *tb, int frame, int *iteraCena) {
    switch (frame) {
    case 1:
        break;

    case 2:
        (*iteraCena)++;
        strcpy(tb->mensagens[0], "Seus hobbies de infancia eram");
        strcpy(tb->mensagens[1], "atirar com seu revolver, subir");
        strcpy(tb->mensagens[2], "escadas e procurar por gemas.");
        break;

    case 3:
        (*iteraCena)++;
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
        if (*iteraCena != 3)
           *iteraCena = 3;
        strcpy(tb->mensagens[0], "Depois disso tudo, o que");
        strcpy(tb->mensagens[1], "sobrou para El Nuestro Cowboy");
        strcpy(tb->mensagens[2], "foram seus antigos talentos.");
        break;

    case 6:
        *iteraCena = 1;
        strcpy(tb->mensagens[0], "Atirar com o revolver.");
        strcpy(tb->mensagens[1], "Subir escadas.");
        strcpy(tb->mensagens[2], "Procurar por gemas preciosas.");
        break;

    case 7:
        *iteraCena = 4;
        strcpy(tb->mensagens[0], "Nao importa quantos cavalos");
        strcpy(tb->mensagens[1], "zumbi ele tenha que exterminar");
        strcpy(tb->mensagens[2], "(ou ignorar) pelo caminho.");
        break;
    }
}
