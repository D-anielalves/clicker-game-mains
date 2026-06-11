#ifndef JOGO_H
#define JOGO_H

#include "structs.h"

// Inicializa os dados do jogo
void inicializarJogo(Jogo *jogo);

// Associa as imagens aos slots
void atribuirImagens(
    Slot slots[],
    ALLEGRO_BITMAP *cpu_img,
    ALLEGRO_BITMAP *gpu_img,
    ALLEGRO_BITMAP *ram_img
);

#endif