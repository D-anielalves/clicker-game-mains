#include "clique.h"
#include "upgrades.h"
#include "pilha.h"
#include "conquistas.h"

// =====================
// CLIQUE
// =====================
void tratarClique(Jogo *jogo,
                  Slot slots[],
                  Pilha *historico,
                  int multiplicador[3][1],
                  Conquista conquistas[],
                  int *conquistaDesbloqueada,
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
    int painel_x = jogo->larguraTela - (BOTAO_W + 90);
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

            *conquistaDesbloqueada = verificarConquistas(jogo, slots, conquistas); 
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

        return;
    }

    // BOTÃO EVOLUIR PC
    int nivelTotal = slots[0].nivel + slots[1].nivel + slots[2].nivel;

    int botaoEvoluir_x = jogo->larguraTela - 400;
    int botaoEvoluir_y = 560;
    int botaoEvoluir_w = 300;
    int botaoEvoluir_h = 110;

    bool podeEvoluir1 = (jogo->evolucaoPc == 0 &&
                         nivelTotal >= 10 &&
                         jogo->pontos >= 8000);

    bool podeEvoluir2 = (jogo->evolucaoPc == 1 &&
                         nivelTotal >= 20 &&
                         jogo->pontos >= 90000);

    if ((podeEvoluir1 || podeEvoluir2) &&
        x >= botaoEvoluir_x &&
        x <= botaoEvoluir_x + botaoEvoluir_w &&
        y >= botaoEvoluir_y &&
        y <= botaoEvoluir_y + botaoEvoluir_h) {

        if (podeEvoluir1) {
            jogo->pontos -= 8000;
            jogo->evolucaoPc = 1;
        } else if (podeEvoluir2) {
            jogo->pontos -= 90000;
            jogo->evolucaoPc = 2;
        }
    }

    // VERIFICAR CONQUISTAS
    *conquistaDesbloqueada = verificarConquistas(jogo, slots, conquistas);
}       
 