#ifndef RENDER_H
#define RENDER_H

#include "structs.h"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

void desenhar(
    Jogo *jogo,
    ALLEGRO_BITMAP *pc,
    ALLEGRO_BITMAP *menu,
    ALLEGRO_BITMAP *cpu_img,
    ALLEGRO_BITMAP *gpu_img,
    ALLEGRO_BITMAP *ram_img,
    ALLEGRO_FONT *font,
    Slot slots[],
    Pilha *historico,
    int multiplicador[3][1],
    int largura_original,
    int altura_original
);

#endif