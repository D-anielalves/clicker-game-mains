#ifndef JOGO_H
#define JOGO_H

#include "structs.h"

void inicializarJogo(Jogo *jogo);

void atribuirImagens(
    Slot slots[],
    ALLEGRO_BITMAP *cpu_img,
    ALLEGRO_BITMAP *gpu_img,
    ALLEGRO_BITMAP *ram_img
);

#endif