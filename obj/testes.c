// Salvando código se precisar voltar atrás
/*
void sobeEscada(PLAYER *p, char mapa[30][30]){
    int posXGrid=(*p).posX/COMP_LINHA;
    int posYGrid=(*p).posY/COMP_COLUNA;
    int subirTiles=0;

    while(mapa[posYGrid][posXGrid]!='D' && mapa[posYGrid][posXGrid+1]!='D'){
        subirTiles+=COMP_COLUNA;
        posYGrid--;
    }

    (*p).posY-=subirTiles;
}

void desceEscada(PLAYER *p, char mapa[30][30]){
    int posXGrid=(*p).posX/COMP_LINHA;
    int posYGrid=(*p).posY/COMP_COLUNA;
    int subirTiles=0;

    while(mapa[posYGrid][posXGrid]!='S' && mapa[posYGrid][posXGrid+1]!='S'){
        subirTiles-=COMP_COLUNA;
        posYGrid++;
    }

    (*p).posY-=subirTiles;
}
*/
