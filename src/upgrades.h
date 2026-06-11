#ifndef UPGRADES_H
#define UPGRADES_H

#include "structs.h"

// Calcula custo com recursividade
int calcularCustoRecursivo(
    int custoBase,
    int nivel
);

// Ordena upgrades por custo
void ordenarUpgrades(
    Slot slots[]
);

// Busca melhor upgrade disponível
Slot* buscarMelhorUpgrade(
    Jogo *jogo,
    Slot slots[]
);

#endif