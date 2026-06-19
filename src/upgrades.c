#include "upgrades.h"

// =====================
// RECURSIVIDADE
// CALCULAR CUSTO
// =====================
int calcularCustoRecursivo(int custoBase, int nivel) {

    // CASO BASE
    if (nivel == 0)
        return custoBase;

    // CHAMADA RECURSIVA
    return(int)(calcularCustoRecursivo(
        custoBase,
        nivel - 1
    ) * 1.3);
}

// =====================
// ORDENAÇÃO BUBBLE SORT
// POR CUSTO
// =====================
void ordenarUpgrades(Slot slots[]) {

    for (int i = 0; i < NUM_SLOTS - 1; i++) {

        for (int j = 0; j < NUM_SLOTS - 1 - i; j++) {

            int custoAtual =
                calcularCustoRecursivo(
                    slots[j].up.custo,
                    slots[j].nivel
                );

            int proximoCusto =
                calcularCustoRecursivo(
                    slots[j + 1].up.custo,
                    slots[j + 1].nivel
                );

            if (custoAtual > proximoCusto) {

                Slot temp = slots[j];

                slots[j] = slots[j + 1];

                slots[j + 1] = temp;
            }
        }
    }
}

// =====================
// BUSCA LINEAR
// MELHOR UPGRADE
// =====================
Slot* buscarMelhorUpgrade(Jogo *jogo, Slot slots[]) {
    Slot *melhor = NULL;
    for (int i = 0; i < NUM_SLOTS; i++) {
        int custoAtual = calcularCustoRecursivo(slots[i].up.custo, slots[i].nivel);
        if (jogo->pontos >= custoAtual) {

            if (melhor == NULL ||
                slots[i].up.bonus > melhor->up.bonus) {

                melhor = &slots[i];
            }
        }
    }
    return melhor;
}