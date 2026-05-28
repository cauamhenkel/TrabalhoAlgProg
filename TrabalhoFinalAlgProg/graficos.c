#include "graficos.h"

void carregaSpritesheet(Spritesheet *sprites) {
    sprites->player = LoadTexture("assets/player.png");
    sprites->monstro = LoadTexture("assets/monstro.png");
    sprites->tileset = LoadTexture("assets/tileset.png");
}

void descarregaSpritesheet(Spritesheet *sprites) {
    UnloadTexture(sprites->player);
    UnloadTexture(sprites->monstro);
    UnloadTexture(sprites->tileset);
}

Rectangle selecionaTile(int coluna, int linha) {
    Rectangle tile = {coluna * TAM_TILE,
                      linha * TAM_TILE,
                      TAM_TILE,
                      TAM_TILE};

    return tile;
}

void desenhaMapa(char mapa[TILES][TILES], Texture2D tileset) {
    Rectangle tile;

    for (int i=0; i<TILES; i++) {
        for (int j=0; j<TILES; j++) {
            switch (mapa[i][j]) {
            case 'F':
                tile = selecionaTile(0, 0);
                break;
            case 'Z':
                tile = selecionaTile(0, 1);
                break;
            case 'S':
            case 'H':
                tile = selecionaTile(1, 1);
                break;
            case 'D':
                tile = selecionaTile(2, 1);
                break;
            case 'X':
                tile = selecionaTile(3, 1);
                break;
            default: continue;
            }
            Rectangle destino = {j * COMP_LINHA,
                                 i * COMP_COLUNA + CABECALHO,
                                 COMP_LINHA,
                                 COMP_COLUNA};
            DrawTexturePro(tileset, tile, destino, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}

void exibeCabecalho(PLAYER p, int fase, int pontos){
/* Essa função desenha as informações do cabeçalho */
    DrawText(TextFormat("Saude: %d", p.saude), COMP_COLUNA/2, (FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
    DrawText(TextFormat("Tiros: %d", p.qtdTiros), COMP_COLUNA/2, (COMP_COLUNA*1.5)+(FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
    // Arrumar essa bizarrice acima depois
    DrawText(TextFormat("Pontos: %d", pontos), (LARGURA/2)-(MeasureText("Pontos : 5000", FONTE_CABECALHO)/2), (FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
    DrawText(TextFormat("Fase atual: %d", fase+1), LARGURA - (MeasureText("Fase atual: 67", FONTE_CABECALHO)), (FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
}
