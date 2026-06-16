#include "menu.h"
#include <allegro5/allegro_primitives.h>

void desenharMenu(Botao botoes[], int total) {

for (int i = 0; i < total; i++) {

   al_draw_scaled_bitmap(
    botoes[i].imagem,
    0, 0,
    al_get_bitmap_width(botoes[i].imagem),
    al_get_bitmap_height(botoes[i].imagem),
    botoes[i].x,
    botoes[i].y,
    botoes[i].largura,
    botoes[i].altura,
    0
); }
}

OpcaoMenu tratarCliqueMenu(Botao botoes[], int total, int mouse_x, int mouse_y) {
    for (int i = 0; i < total; i++) {
        if (mouse_x >= botoes[i].x &&
            mouse_x <= botoes[i].x + botoes[i].largura &&
            mouse_y >= botoes[i].y &&
            mouse_y <= botoes[i].y + botoes[i].altura) {

            return botoes[i].acao;
        }
    }
    return MENU_NENHUM;
}