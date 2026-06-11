#include "clique.h"
#include "upgrades.h"
#include "pilha.h"

// =====================
// CLIQUE
// =====================
void tratarClique(Jogo *jogo,
                  Slot slots[],
                  Pilha *historico,
                  int multiplicador[3][1],
                  int x, int y) {

    int mult = multiplicador[jogo->opcaoCompra][0];

    // CLICK PC
    int margem = jogo->img_w * 0.15;

    if (x >= jogo->img_x + margem &&
        x <= jogo->img_x + jogo->img_w - margem &&
        y >= jogo->img_y + margem &&
        y <= jogo->img_y + jogo->img_h - margem) {

        jogo->pontos += jogo->pontosPorClique;
        return;
    }

    // UPGRADES
    int painel_x = jogo->larguraTela - 400;
    int start_y = 150;

    for (int i = 0; i < NUM_SLOTS; i++) {

        int by = start_y + (i * (BOTAO_H + ESPACAMENTO));

        if (x >= painel_x &&
            x <= painel_x + BOTAO_W &&
            y >= by &&
            y <= by + BOTAO_H) {

            for (int j = 0; j < mult; j++) {

    int custoAtual =
        calcularCustoRecursivo(
            slots[i].up.custo,
            slots[i].nivel
        );

    if (jogo->pontos >= custoAtual) {

        jogo->pontos -= custoAtual;

        jogo->pontosPorClique +=
            slots[i].up.bonus;

        slots[i].nivel++;

        pushCompra(
            historico,
            slots[i].up.nome
);

        ordenarUpgrades(slots);
    }
}

            return;
        }
    }

    // TROCAR MODO DE COMPRA
    float centro_pc_x = jogo->img_x + (jogo->img_w / 2) - 180;
    float base_pc_y = jogo->img_y + jogo->img_h - 60;

    float modo_y = base_pc_y + 30;

    if (x >= centro_pc_x - 100 &&
        x <= centro_pc_x + 100 &&
        y >= modo_y - 15 &&
        y <= modo_y + 25) {

        jogo->opcaoCompra++;

        if (jogo->opcaoCompra > MULT_10X)
            jogo->opcaoCompra = MULT_1X;
    }
}                  
 