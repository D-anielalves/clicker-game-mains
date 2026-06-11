#include "jogo.h"

void inicializarJogo(Jogo *jogo)
{
    jogo->pontos = 0;
    jogo->pontosPorClique = 1;

    jogo->larguraTela = 1280;
    jogo->alturaTela = 720;

    jogo->img_x = 0;
    jogo->img_y = 0;

    jogo->img_w = 0;
    jogo->img_h = 0;

    jogo->opcaoCompra = MULT_1X;
}

void atribuirImagens(
    Slot slots[],
    ALLEGRO_BITMAP *cpu_img,
    ALLEGRO_BITMAP *gpu_img,
    ALLEGRO_BITMAP *ram_img
)
{
    slots[0].img = cpu_img;
    slots[1].img = gpu_img;
    slots[2].img = ram_img;
}