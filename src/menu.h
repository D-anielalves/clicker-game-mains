#ifndef MENU_H
#define MENU_H

#include "structs.h"

void desenharMenu(Botao botoes[], int total);
OpcaoMenu tratarCliqueMenu(Botao botoes[], int total, int mouse_x, int mouse_y);

#endif