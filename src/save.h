#ifndef SAVE_H
#define SAVE_H

#include "structs.h"
#include "conquistas.h"

void salvarJogo(
    Jogo *jogo,
    Slot slots[],
    Conquista conquistas[]
);

void carregarJogo(
    Jogo *jogo,
    Slot slots[],
    Conquista conquistas[]
);

#endif