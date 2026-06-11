#ifndef CLIQUE_H
#define CLIQUE_H

#include "structs.h"

void tratarClique(
    Jogo *jogo,
    Slot slots[],
    Pilha *historico,
    int multiplicador[3][1],
    int x,
    int y
);

#endif