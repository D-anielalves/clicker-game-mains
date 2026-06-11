#include "save.h"
#include <stdio.h>

void salvarJogo(Jogo *jogo, Slot slots[]) {

    FILE *arquivo = fopen("save.txt", "w");

    if (!arquivo) {
        printf("Erro ao salvar jogo\n");
        return;
    }

    fprintf(arquivo, "%d\n", jogo->pontos);
    fprintf(arquivo, "%d\n", jogo->pontosPorClique);
    fprintf(arquivo, "%d\n", jogo->opcaoCompra);

    for (int i = 0; i < NUM_SLOTS; i++) {
        fprintf(arquivo, "%d\n", slots[i].nivel);
    }

    fclose(arquivo);

    printf("Jogo salvo!\n");
}

// =====================
// LOAD
// =====================
void carregarJogo(Jogo *jogo, Slot slots[]) {

    FILE *arquivo = fopen("save.txt", "r");

    if (!arquivo) {

    printf("Criando novo save...\n");

    salvarJogo(jogo, slots);

    return;
}

    fscanf(arquivo, "%d", &jogo->pontos);
    fscanf(arquivo, "%d", &jogo->pontosPorClique);
    fscanf(arquivo, "%d", (int*)&jogo->opcaoCompra);

    for (int i = 0; i < NUM_SLOTS; i++) {
        fscanf(arquivo, "%d", &slots[i].nivel);
    }

    fclose(arquivo);

    printf("Save carregado!\n");
}