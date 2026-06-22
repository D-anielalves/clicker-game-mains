#ifndef CLIQUE_H
#define CLIQUE_H

#include "structs.h"
#include "conquistas.h"

void tratarClique(
    Jogo *jogo,
    Slot slots[],
    Pilha *historico,
    int multiplicador[3][1],
    Conquista conquistas[],
    int *conquistaDesbloqueada,
    int x,
    int y
);
int tratarCliqueConquistas(
    Jogo *jogo,
    Conquista conquistas[],
    int x,
    int y
);

#endif