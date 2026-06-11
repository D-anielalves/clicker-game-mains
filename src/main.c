#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "structs.h"
#include "pilha.h"
#include "jogo.h"
#include "save.h"
#include "upgrades.h"
#include "clique.h"
#include "render.h"

#define NUM_SLOTS 3
#define BOTAO_W 350
#define BOTAO_H 120
#define ESPACAMENTO 20


// =====================
// MAIN
// =====================
int main() {

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_EVENT event;

    ALLEGRO_TIMER *timerSave = NULL;

    ALLEGRO_BITMAP *pc = NULL;
    ALLEGRO_BITMAP *menu = NULL;

    ALLEGRO_BITMAP *cpu_img = NULL;
    ALLEGRO_BITMAP *gpu_img = NULL;
    ALLEGRO_BITMAP *ram_img = NULL;

    ALLEGRO_FONT *font = NULL;

    Jogo *jogo = malloc(sizeof(Jogo));
   
    Pilha historico;
    inicializarPilha(&historico);

    if (!jogo)
        return -1;


    inicializarJogo(jogo);

    Slot slots[NUM_SLOTS];

    // CPU
    strcpy(slots[0].up.nome, "CPU");
    slots[0].up.custo = 10;
    slots[0].up.bonus = 1;
    slots[0].nivel = 0;
    slots[0].img = NULL;

    // GPU
    strcpy(slots[1].up.nome, "GPU");
    slots[1].up.custo = 50;
    slots[1].up.bonus = 5;
    slots[1].nivel = 0;
    slots[1].img = NULL;

    // RAM
    strcpy(slots[2].up.nome, "RAM");
    slots[2].up.custo = 100;
    slots[2].up.bonus = 10;
    slots[2].nivel = 0;
    slots[2].img = NULL;

    // LOAD SAVE
    carregarJogo(jogo, slots);

    // MATRIZ
    int multiplicador[3][1] = {
        {1},
        {5},
        {10}
    };

    // ALLEGRO
    if (!al_init())
        return -1;

    al_install_mouse();

    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    timerSave = al_create_timer(10.0);

if (!timerSave) {
    printf("Erro timer save\n");
    return -1;
    }
    
    display = al_create_display(
        jogo->larguraTela,
        jogo->alturaTela
    );

    if (!display) {
        printf("Erro display\n");
        return -1;
    }

    // FONT
    font = al_load_ttf_font(
        "../assets/fonts/ari-w9500.ttf",
        20,
        0
    );

    if (!font) {
        printf("Erro fonte\n");
        return -1;
    }

    // IMAGENS
    pc = al_load_bitmap("../assets/images/pc.png");
    menu = al_load_bitmap("../assets/images/menu.png");

    cpu_img = al_load_bitmap("../assets/images/upgrade_CPU.png");
    gpu_img = al_load_bitmap("../assets/images/upgrade_GPU.png");
    ram_img = al_load_bitmap("../assets/images/upgrade_RAM.png");

    atribuirImagens(
    slots,
    cpu_img,
    gpu_img,
    ram_img
);

    if (!pc || !menu ||
        !cpu_img || !gpu_img || !ram_img) {

        printf("Erro imagens\n");
        return -1;
    }

    // TAMANHO PC
    int largura_original = al_get_bitmap_width(pc);
    int altura_original = al_get_bitmap_height(pc);

    jogo->img_w = 780;

    jogo->img_h =
        (altura_original * jogo->img_w)
        / largura_original;

    jogo->img_x =
        (jogo->larguraTela / 2)
        - (jogo->img_w / 2);

    jogo->img_y = 40;

    // EVENTOS
    queue = al_create_event_queue();

    al_register_event_source(
        queue,
        al_get_mouse_event_source()
    );

    al_register_event_source(
        queue,
        al_get_display_event_source(display)
    );

    al_register_event_source(
    queue,
    al_get_timer_event_source(timerSave)
    );

    al_start_timer(timerSave);

    bool rodando = true;

    // LOOP
    while (rodando) {

        desenhar(
            jogo,
            pc,
            menu,
            cpu_img,
            gpu_img,
            ram_img,
            font,
            slots,
            &historico,
            multiplicador,
            largura_original,
            altura_original
        );

        al_wait_for_event(queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            salvarJogo(jogo, slots);

            rodando = false;
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {

            salvarJogo(jogo, slots);

            printf("Auto save!\n");
}

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

            tratarClique(
                jogo,
                slots,
                &historico,
                multiplicador,
                event.mouse.x,
                event.mouse.y
);
        }
    }

    // LIMPEZA
    al_destroy_bitmap(pc);
    al_destroy_bitmap(menu);

    al_destroy_bitmap(cpu_img);
    al_destroy_bitmap(gpu_img);
    al_destroy_bitmap(ram_img);

    al_destroy_font(font);

    al_destroy_event_queue(queue);
    al_destroy_timer(timerSave);
    al_destroy_display(display);

    free(jogo);

    return 0;
}