#ifndef CONQUISTAS_H
#define CONQUISTAS_H
 
#include "structs.h"
 
#define NUM_CONQUISTAS 4
 
// Índices das conquistas (pra facilitar referência no código)
typedef enum {
    CONQ_CPU_50 = 0,
    CONQ_GPU_50 = 1,
    CONQ_RAM_50 = 2,
    CONQ_EVOLUCAO_1 = 3
} IndiceConquista;
 
// Inicializa o array de conquistas (nomes, descrições, desbloqueada=false)
void inicializarConquistas(Conquista conquistas[]);
 
// Carrega as imagens das conquistas nos bitmaps já alocados
void carregarImagensConquistas(Conquista conquistas[],
                                ALLEGRO_BITMAP *cpu50,
                                ALLEGRO_BITMAP *gpu50,
                                ALLEGRO_BITMAP *ram50,
                                ALLEGRO_BITMAP *evolucao1);
 
// Verifica todas as condições e desbloqueia conquistas novas.
// Retorna o índice da conquista desbloqueada agora (-1 se nenhuma)
int verificarConquistas(Jogo *jogo, Slot slots[], Conquista conquistas[]);
 
#endif
 