#include "graficos.h"
#include "funcoesGerais.h"
#include "monstro.h"
#include "player.h"
#include "raylib.h"

void carregaSpritesheet(Spritesheet *sprites) {
    sprites->player = LoadTexture("assets/player.png");
    sprites->monstro = LoadTexture("assets/monstro.png");
    sprites->tileset = LoadTexture("assets/tileset_2.png");
}

void descarregaSpritesheet(Spritesheet *sprites) {
    UnloadTexture(sprites->player);
    UnloadTexture(sprites->monstro);
    UnloadTexture(sprites->tileset);
}

Rectangle selecionaTile(int coluna, int linha, int tam_tile) {
    Rectangle tile = {coluna * tam_tile,
                      linha * tam_tile,
                      tam_tile,
                      tam_tile};

    return tile;
}

Rectangle selecionaTileInverso(int coluna, int linha, int tam_tile) {
    Rectangle tile = {coluna * tam_tile,
                      linha * tam_tile,
                      -tam_tile,
                      tam_tile};

    return tile;
}

void desenhaMapa(char mapa[TILES][TILES], Texture2D tileset) {
    Rectangle tile, destino;

    for (int i=0; i<TILES; i++) {
        for (int j=0; j<TILES; j++) {
            switch (mapa[i][j]) {
            case 'F':
                tile = selecionaTile(0, 0, 16);
                break;
            case 'Z':
                tile = selecionaTile(0, 1, 16);
                break;
            case 'S':
            case 'H':
                tile = selecionaTile(1, 1, 16);
                break;
            case 'D':
                tile = selecionaTile(2, 1, 16);
                break;
            case 'X':
                tile = selecionaTile(3, 1, 16);
                break;
            default: continue;
            }

            if (mapa[i][j] == 'F') {
                destino = (Rectangle) {j * COMP_LINHA -(COMP_LINHA/4),
                                    i * COMP_COLUNA + CABECALHO -(COMP_COLUNA/2),
                                    COMP_LINHA +(COMP_LINHA/2),
                                    COMP_COLUNA +(COMP_COLUNA/2)};
            } // Faz com que a joia do final seja um pouco maior que o tile normal (questoes esteticas)
            else {
                destino = (Rectangle) {j * COMP_LINHA,
                                    i * COMP_COLUNA + CABECALHO,
                                    COMP_LINHA,
                                    COMP_COLUNA};
            }

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

void atualizaAnimacaoPlayer(PLAYER *p) {
    p->animacaoTimer += GetFrameTime();
    if (p->animacaoTimer >= 1.0f / 8.0f) { // 8 Frames Por Segundo
        p->animacaoTimer = 0; // Reseta o timer

        if (p->noChao && (p->velX > 0.6 || p->velX < -0.6))
            p->frameAtual = (p->frameAtual + 1) % 3; // Animacao de caminhar tem 3 frames
        else if (p->naEscada && (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)))
            p->frameAtual = (p->frameAtual + 1) % 2; // Animacao de escada tem 2 frames
        else
            p->frameAtual =0;
    }
}

Rectangle selecionaFramePlayer(PLAYER p) {
    Rectangle fonte;

    if (p.naEscada)
        fonte = selecionaTile(p.frameAtual, 2, 16);
    else if (p.dir == DIREITA) {
        if (p.noChao) {
            if (p.velX > 0.6)
                fonte = selecionaTile(p.frameAtual, 1, 16);
            else
                fonte = selecionaTile(0, 0, 16);
        }
        else
            fonte = selecionaTile(1, 0, 16);
    }
    else if (p.dir == ESQUERDA) {
        if (p.noChao) {
            if (p.velX < -0.6)
                fonte = selecionaTileInverso(p.frameAtual, 1, 16);
            else
                fonte = selecionaTileInverso(0, 0, 16);
        }
        else
            fonte = selecionaTileInverso(1, 0, 16);
    }

    return fonte;
}

void desenhaPlayer(PLAYER p, Texture2D sprite){
    Rectangle fonte = selecionaFramePlayer(p);
    Rectangle destino = {p.posX -(COMP_LINHA/8),
                         p.posY + CABECALHO -(COMP_COLUNA/4),
                         COMP_LINHA +(COMP_LINHA/4),
                         COMP_COLUNA +(COMP_COLUNA/4)}; // Retangulo referente ao sprite exibido na tela

    if (p.invencibilidade % 10 > 5) // Player pisca quando toma dano
        DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, RED);
    else
        DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, WHITE); // Funcao escala automaticamente o retangulo da fonte para o do destino
}

void atualizaAnimacaoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros) {
    for (int i=0; i<qtdMonstros; i++) {
        monstros[i].animacaoTimer += GetFrameTime();
        if (monstros[i].animacaoTimer >= 1.0f / 8.0f) { // 8 Frames Por Segundo
            monstros[i].animacaoTimer = 0; // Reseta o timer

            monstros[i].frameAtual = (monstros[i].frameAtual + 1) % 8; // Animacao do cavalo zumbi andando tem 8 frames
        }
    }
}

Rectangle selecionaFrameMonstro(MONSTRO monstro) {
    Rectangle fonte;

    if (monstro.dir == ESQUERDA)
        fonte = selecionaTile(monstro.frameAtual, 0, 16);
    else
        fonte = selecionaTileInverso(monstro.frameAtual, 0, 16);

    return fonte;
}

void desenhaMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, Texture2D sprite){
/* Essa função desenha os monstros usando suas posições */
    for (int i=0 ; i<qtdMonstros ; i++){    // Itera um número de vezes igual a quantidade de monstros
        if (monstros[i].estadoMonstro==ATIVO){
            Rectangle fonte = selecionaFrameMonstro(monstros[i]); // Retangulo referente ao sprite original
            Rectangle destino = {monstros[i].posX -(COMP_LINHA/8),
                                monstros[i].posY + CABECALHO -(COMP_LINHA/4),
                                COMP_LINHA +(COMP_LINHA/4),
                                COMP_COLUNA +(COMP_LINHA/4)}; // Retangulo referente ao sprite exibido na tela
            DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}
