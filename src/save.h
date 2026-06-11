#ifndef SAVE_H
#define SAVE_H

#include "structs.h"

void salvarJogo(
    Jogo *jogo,
    Slot slots[]
);

void carregarJogo(
    Jogo *jogo,
    Slot slots[]
);

#endif