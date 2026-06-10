#include "raylib.h"
#include "graficos.h"
#include "funcoesGerais.h"
#include "player.h"
#include "monstro.h"

void carregaSpritesheet(Spritesheet *sprites){
/* Essa função carrega os sprites da memória para a variável do código */
    sprites->player = LoadTexture("assets/player.png");
    sprites->monstro = LoadTexture("assets/monstro.png");
    sprites->tileset = LoadTexture("assets/tileset_2.png");
}

void descarregaSpritesheet(Spritesheet *sprites){
/* Essa função libera os sprites que foram carregados no início */
    UnloadTexture(sprites->player);
    UnloadTexture(sprites->monstro);
    UnloadTexture(sprites->tileset);
}

void carregaSoundtrack(Soundtrack *sounds){
/* Essa função carrega os sons da memória para a variável do código */
    sounds->botao = LoadSound("assets/audio/botao.wav");
    sounds->tiro = LoadSound("assets/audio/tiro.wav");
    sounds->monstro_dano = LoadSound("assets/audio/monstrodano.wav");
}

void descarregaSoundtrack(Soundtrack *sounds){
/* Essa função libera os sons que foram carregados no início */
    UnloadSound(sounds->botao);
    UnloadSound(sounds->tiro);
    UnloadSound(sounds->monstro_dano);
}

Rectangle selecionaTile(int coluna, int linha, int tam_tile){
/* Essa função constrói um retângulo usando os valores de entrada como referência */
    Rectangle tile = {coluna * tam_tile,
                      linha * tam_tile,
                      tam_tile,
                      tam_tile};

    return tile;
}

Rectangle selecionaTileInverso(int coluna, int linha, int tam_tile){
/* Não faço ideia, comenta melhor teus código Mateus, eu sou burro */ ////////////////////////////////////////////////////////////////////////////
    Rectangle tile = {coluna * tam_tile,
                      linha * tam_tile,
                      -tam_tile,
                      tam_tile};

    return tile;
}

void desenhaMapa(char mapa[TILES][TILES], Texture2D tileset){
/* Essa função desenha o mapa usando o grid salvo como referência e um tileset */
    Rectangle tile, destino;
// Comenta isso aqui mateus eu sei la que porra tu ta fazendo
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
            // Faz com que a joia do final seja um pouco maior que o tile normal (questoes esteticas)
            if (mapa[i][j] == 'F') {
                destino = (Rectangle) {j * COMP_LINHA -(COMP_LINHA/4),
                                    i * COMP_COLUNA + CABECALHO -(COMP_COLUNA/2),
                                    COMP_LINHA +(COMP_LINHA/2),
                                    COMP_COLUNA +(COMP_COLUNA/2)};
            }
            else {
                // Tipo, que merda é essa ////////////////////////////////////////////////////////////////////////////////////////////
                destino = (Rectangle) {j * COMP_LINHA,
                                    i * COMP_COLUNA + CABECALHO,
                                    COMP_LINHA,
                                    COMP_COLUNA};
            }
            // WTF ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            DrawTexturePro(tileset, tile, destino, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}

void exibeCabecalho(PLAYER p, int fase, int pontos){
/* Essa função desenha as informações do cabeçalho */
    DrawText(TextFormat("Saude: %d", p.saude), COMP_COLUNA/2, (FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
    DrawText(TextFormat("Tiros: %d", p.qtdTiros), COMP_COLUNA/2, (COMP_COLUNA*1.5)+(FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
    DrawText(TextFormat("Pontos: %d", pontos), (LARGURA/2)-(MeasureText("Pontos : 5000", FONTE_CABECALHO)/2), (FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
    DrawText(TextFormat("Fase atual: %d", fase+1), LARGURA - (MeasureText("Fase atual: 67", FONTE_CABECALHO)), (FONTE_BOTOES / 2), FONTE_CABECALHO, RED);
}

void atualizaAnimacaoPlayer(PLAYER *p){
/* Essa função atualiza a animação do player usando os frames que se passaram como referência */
    p->animacaoTimer += GetFrameTime();
    // 8 Frames Por Segundo
    if (p->animacaoTimer >= 1.0f / 8.0f){
        // Reseta o timer
        p->animacaoTimer = 0;

        if (p->noChao && (p->velX > 0.6 || p->velX < -0.6))
            // Animacao de caminhar tem 3 frames
            p->frameAtual = (p->frameAtual + 1) % 3;
        else if (p->naEscada && (IsKeyDown(KEY_W) || IsKeyDown(KEY_S)))
            // Animacao de escada tem 2 frames
            p->frameAtual = (p->frameAtual + 1) % 2;
        else
            p->frameAtual =0;
    }
}

Rectangle selecionaFramePlayer(PLAYER p){
/* Sei la que porra, Mateus comente isso aqui depois */ ///////////////////////////////////////////////////////////////////////////////
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
/* Essa função desenha o player usando sua posição */
    // Retangulo referente ao sprite original
    Rectangle fonte = selecionaFramePlayer(p);
    // Retangulo referente ao sprite exibido na tela
    Rectangle destino = {p.posX -(COMP_LINHA/8),
                         p.posY + CABECALHO -(COMP_COLUNA/4),
                         COMP_LINHA +(COMP_LINHA/4),
                         COMP_COLUNA +(COMP_COLUNA/4)};
    // Player pisca quando toma dano
    // Funcao escala automaticamente o retangulo da fonte para o do destino
    if (p.invencibilidade % 10 > 5)
        DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, RED);
    else
        DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, WHITE);
}

void atualizaAnimacaoMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros){
/* Essa função atualiza a animação dos monstros usando os frames que se passaram como referência */
    for (int i=0; i<qtdMonstros; i++) {
        monstros[i].animacaoTimer += GetFrameTime();
        // 8 Frames Por Segundo
        if (monstros[i].animacaoTimer >= 1.0f / 8.0f){
            // Reseta o timer
            monstros[i].animacaoTimer = 0;
            // Animacao do cavalo zumbi andando tem 8 frames
            monstros[i].frameAtual = (monstros[i].frameAtual + 1) % 8;
        }
    }
}

Rectangle selecionaFrameMonstro(MONSTRO monstro){
/* Sei lá não entendo SOS Mateus */ //////////////////////////////////////////////////////////////////////////////////////////
    Rectangle fonte;

    if (monstro.dir == ESQUERDA)
        fonte = selecionaTile(monstro.frameAtual, 0, 16);
    else
        fonte = selecionaTileInverso(monstro.frameAtual, 0, 16);

    return fonte;
}

void desenhaMonstros(MONSTRO monstros[M_QTD_MAX], int qtdMonstros, Texture2D sprite){
/* Essa função desenha os monstros usando suas posições */
    // Itera um número de vezes igual a quantidade de monstros
    for (int i=0 ; i<qtdMonstros ; i++){
        // Desenha apenas se eles não estiverem mortos
        if (monstros[i].estadoMonstro==ATIVO){
            // Retangulo referente ao sprite original
            Rectangle fonte = selecionaFrameMonstro(monstros[i]);
            // Retangulo referente ao sprite exibido na tela
            Rectangle destino = {monstros[i].posX -(COMP_LINHA/8),
                                monstros[i].posY + CABECALHO -(COMP_LINHA/4),
                                COMP_LINHA +(COMP_LINHA/4),
                                COMP_COLUNA +(COMP_LINHA/4)};
            // Desenha o monstro na tela
            DrawTexturePro(sprite, fonte, destino, (Vector2){0,0}, 0.0f, WHITE);
        }
    }
}
