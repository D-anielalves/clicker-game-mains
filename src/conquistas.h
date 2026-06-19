#ifndef CONQUISTAS_H
#define CONQUISTAS_H
 
#include "structs.h"
 
#define NUM_CONQUISTAS 4
 
typedef enum {
    CONQ_CPU_50 = 0,
    CONQ_GPU_50 = 1,
    CONQ_RAM_50 = 2,
    CONQ_EVOLUCAO_1 = 3
} IndiceConquista;
 
void inicializarConquistas(Conquista conquistas[]);
 
void carregarImagensConquistas(Conquista conquistas[],
                                ALLEGRO_BITMAP *cpu50,
                                ALLEGRO_BITMAP *gpu50,
                                ALLEGRO_BITMAP *ram50,
                                ALLEGRO_BITMAP *evolucao1);
 

int verificarConquistas(Jogo *jogo, Slot slots[], Conquista conquistas[]);
 
#endif
 