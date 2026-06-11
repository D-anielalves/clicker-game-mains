#include "pilha.h"
#include <string.h>

void inicializarPilha(Pilha *pilha)
{
    pilha->topo = -1;
}

void pushCompra(
    Pilha *pilha,
    char nome[]
)
{
    if (pilha->topo >= 99)
        return;

    pilha->topo++;

    strcpy(
        pilha->itens[pilha->topo].nome,
        nome
    );
}