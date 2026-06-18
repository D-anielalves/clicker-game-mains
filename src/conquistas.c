#include "conquistas.h"
#include <string.h>
 
// =====================
// INICIALIZAR
// =====================
void inicializarConquistas(Conquista conquistas[]) {
 
    strcpy(conquistas[CONQ_CPU_50].nome, "Conquista 50lv de Cpu");
    strcpy(conquistas[CONQ_CPU_50].descricao, "Sera que e kit xeon?");
    conquistas[CONQ_CPU_50].desbloqueada = false;
    conquistas[CONQ_CPU_50].img = NULL;
 
    strcpy(conquistas[CONQ_GPU_50].nome, "Conquista 50lv de Gpu");
    strcpy(conquistas[CONQ_GPU_50].descricao, "Vai minerar Bitcoin patrao?");
    conquistas[CONQ_GPU_50].desbloqueada = false;
    conquistas[CONQ_GPU_50].img = NULL;
 
    strcpy(conquistas[CONQ_RAM_50].nome, "Conquista 50lv de Ram");
    strcpy(conquistas[CONQ_RAM_50].descricao, "DDR5 ta caro se liga doidao.");
    conquistas[CONQ_RAM_50].desbloqueada = false;
    conquistas[CONQ_RAM_50].img = NULL;
 
    strcpy(conquistas[CONQ_EVOLUCAO_1].nome, "Faca sua primeira evolucao de PC");
    strcpy(conquistas[CONQ_EVOLUCAO_1].descricao, "Sera que agora roda Red Dead?");
    conquistas[CONQ_EVOLUCAO_1].desbloqueada = false;
    conquistas[CONQ_EVOLUCAO_1].img = NULL;
}
 
// =====================
// CARREGAR IMAGENS
// =====================
void carregarImagensConquistas(Conquista conquistas[],
                                ALLEGRO_BITMAP *cpu50,
                                ALLEGRO_BITMAP *gpu50,
                                ALLEGRO_BITMAP *ram50,
                                ALLEGRO_BITMAP *evolucao1) {
 
    conquistas[CONQ_CPU_50].img = cpu50;
    conquistas[CONQ_GPU_50].img = gpu50;
    conquistas[CONQ_RAM_50].img = ram50;
    conquistas[CONQ_EVOLUCAO_1].img = evolucao1;
}
 
// =====================
// VERIFICAR
// =====================
int verificarConquistas(Jogo *jogo, Slot slots[], Conquista conquistas[]) {
 
    // slots[0]=CPU, slots[1]=GPU, slots[2]=RAM (ordem de inicializacao em main.c)
    // OBS: ordenarUpgrades() reordena o array por custo, entao usamos o nome
    // para identificar cada slot em vez do indice fixo.
 
    for (int i = 0; i < NUM_SLOTS; i++) {
        if (strcmp(slots[i].up.nome, "CPU") == 0 &&
            slots[i].nivel >= 50 &&
            !conquistas[CONQ_CPU_50].desbloqueada) {
 
            conquistas[CONQ_CPU_50].desbloqueada = true;
            return CONQ_CPU_50;
        }
 
        if (strcmp(slots[i].up.nome, "GPU") == 0 &&
            slots[i].nivel >= 50 &&
            !conquistas[CONQ_GPU_50].desbloqueada) {
 
            conquistas[CONQ_GPU_50].desbloqueada = true;
            return CONQ_GPU_50;
        }
 
        if (strcmp(slots[i].up.nome, "RAM") == 0 &&
            slots[i].nivel >= 50 &&
            !conquistas[CONQ_RAM_50].desbloqueada) {
 
            conquistas[CONQ_RAM_50].desbloqueada = true;
            return CONQ_RAM_50;
        }
    }
 
    if (jogo->evolucaoPc >= 1 &&
        !conquistas[CONQ_EVOLUCAO_1].desbloqueada) {
 
        conquistas[CONQ_EVOLUCAO_1].desbloqueada = true;
        return CONQ_EVOLUCAO_1;
    }
 
    return -1;
}
 