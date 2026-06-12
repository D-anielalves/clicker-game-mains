#ifndef STRUCTS_H
#define STRUCTS_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

#define NUM_SLOTS 3
#define BOTAO_W 350
#define BOTAO_H 120
#define ESPACAMENTO 20

// =====================
// ENUM
// =====================
typedef enum {
    MULT_1X = 0,
    MULT_5X = 1,
    MULT_10X = 2
} ModoCompra;

// =====================
// STRUCT JOGO
// =====================
typedef struct {
    int pontos;
    int pontosPorClique;

    int larguraTela;
    int alturaTela;

    float img_x;
    float img_y;

    int img_w;
    int img_h;

    ModoCompra opcaoCompra;

} Jogo;

// =====================
// STRUCT UPGRADE
// =====================
typedef struct {
    char nome[50];
    int custo;
    int bonus;
} Upgrade;

// =====================
// SLOT
// =====================
typedef struct {
    Upgrade up;
    int nivel;
    ALLEGRO_BITMAP *img;
} Slot;

// =====================
// HISTÓRICO
// =====================
typedef struct {
    char nome[50];
} Compra;

typedef struct {
    Compra itens[100];
    int topo;
} Pilha;
//========================
// MENU
//========================
typedef enum {
    MENU_JOGAR,
    MENU_CONQUISTAS,
    MENU_SAIR,
    MENU_NENHUM
} OpcaoMenu;

typedef struct {
    float x, y;
    float largura, altura;
    ALLEGRO_BITMAP *imagem;
    OpcaoMenu acao;
} Botao;

typedef enum {
    ESTADO_MENU,
    ESTADO_JOGO,
    ESTADO_CONQUISTAS
} EstadoJogo;

#endif