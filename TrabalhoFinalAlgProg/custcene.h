#ifndef CUTSCENE_H
#define CUTSCENE_H

#include "funcoesGerais.h"

#define MAX_CH 51
#define TB_LINES 4

typedef struct {
    char linhas[TB_LINES][MAX_CH];
    char mensagens[TB_LINES][MAX_CH];
    int posY[TB_LINES];
    double timer;
    int iterador;
    int linhaAtual;
} TextBox;

void cutscene(void);
void writeLine(TextBox *tb, int *frame);
void updateLine(TextBox *tb, int frame);

#endif
