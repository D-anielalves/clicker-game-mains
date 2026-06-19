#include "save.h"
#include "jogo.h"
#include <stdio.h>

void salvarJogo(Jogo *jogo, Slot slots[], Conquista conquistas[]) {

    FILE *arquivo = fopen("save.txt", "w");

    if (!arquivo) {
        printf("Erro ao salvar jogo\n");
        return;
    }

    fprintf(arquivo, "%d\n", jogo->pontos);
    fprintf(arquivo, "%d\n", jogo->pontosPorClique);
    fprintf(arquivo, "%d\n", jogo->opcaoCompra);
    fprintf(arquivo, "%d\n", jogo->evolucaoPc);

    for (int i = 0; i < NUM_SLOTS; i++) {
        fprintf(arquivo, "%d\n", slots[i].nivel);
    }

    for (int i = 0; i < NUM_CONQUISTAS; i++) {
        fprintf(arquivo, "%d\n", conquistas[i].desbloqueada);
    }

    fclose(arquivo);

    printf("Jogo salvo!\n");
}

// =====================
// LOAD
// =====================
void carregarJogo(Jogo *jogo, Slot slots[], Conquista conquistas[]) {

    FILE *arquivo = fopen("save.txt", "r");

    if (!arquivo) {

    printf("Criando novo save...\n");

    salvarJogo(jogo, slots, conquistas);

    return;
}

    int ok = 1;

    ok &= (fscanf(arquivo, "%d", &jogo->pontos) == 1);
    ok &= (fscanf(arquivo, "%d", &jogo->pontosPorClique) == 1);
    ok &= (fscanf(arquivo, "%d", (int*)&jogo->opcaoCompra) == 1);
    ok &= (fscanf(arquivo, "%d", &jogo->evolucaoPc) == 1);

    for (int i = 0; i < NUM_SLOTS; i++) {
        ok &= (fscanf(arquivo, "%d", &slots[i].nivel) == 1);
    }

    for (int i = 0; i < NUM_CONQUISTAS; i++) {
        int valor;
        ok &= (fscanf(arquivo, "%d", &valor) == 1);
        conquistas[i].desbloqueada = valor;
    }

    fclose(arquivo);

    if (!ok) {
        printf("Save corrompido! Reiniciando jogo do zero...\n");
        inicializarJogo(jogo);
        for (int i = 0; i < NUM_SLOTS; i++) {
            slots[i].nivel = 0;
        }
        for (int i = 0; i < NUM_CONQUISTAS; i++) {
            conquistas[i].desbloqueada = false;
        }
        return;
    }

    printf("Save carregado!\n");
}