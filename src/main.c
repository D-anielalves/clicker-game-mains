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
#include "menu.h"
#include "conquistas.h"

#define NUM_SLOTS 3
#define BOTAO_W 350
#define BOTAO_H 120
#define ESPACAMENTO 20


// =====================
// MAIN
// =====================
int main() {
    EstadoJogo estado = ESTADO_MENU;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;
    ALLEGRO_EVENT event;

    ALLEGRO_TIMER *timerSave = NULL;
    ALLEGRO_TIMER *timerRender = NULL;

    ALLEGRO_BITMAP *pc = NULL;
    ALLEGRO_BITMAP *menu = NULL;

    ALLEGRO_BITMAP *cpu_img = NULL;
    ALLEGRO_BITMAP *gpu_img = NULL;
    ALLEGRO_BITMAP *ram_img = NULL;

    ALLEGRO_FONT *font = NULL;

    Jogo *jogo = malloc(sizeof(Jogo));
   
    Pilha historico;
    inicializarPilha(&historico);

    Conquista conquistas[NUM_CONQUISTAS];
    inicializarConquistas(conquistas);

    int conquistaParaMostrar = -1;
    float tempoPopup = 0;
    int conquistaEmZoom = -1;

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
    carregarJogo(jogo, slots, conquistas);

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
    al_install_keyboard();
    al_init_image_addon();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();

    timerSave = al_create_timer(10.0);

if (!timerSave) {
    printf("Erro timer save\n");
    return -1;
    }
    timerRender = al_create_timer(1.0 / 60.0);

if (!timerRender) {
    printf("Erro timer render\n");
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
    ALLEGRO_BITMAP *bg_menu = al_load_bitmap("../assets/images/menu_inicial.png");
    ALLEGRO_BITMAP *btn_jogar = al_load_bitmap("../assets/images/jogar.png");
    ALLEGRO_BITMAP *btn_conquistas = al_load_bitmap("../assets/images/conquistas.png");
    ALLEGRO_BITMAP *btn_sair = al_load_bitmap("../assets/images/sair.png");
    ALLEGRO_BITMAP *pc_evoluido1 = al_load_bitmap("../assets/images/pccomum.png");
    ALLEGRO_BITMAP *pc_evoluido2 = al_load_bitmap("../assets/images/pcgamer.png");
    ALLEGRO_BITMAP *btn_evoluir = al_load_bitmap("../assets/images/evoluirpc.png");

    ALLEGRO_BITMAP *conq_cpu50 = al_load_bitmap("../assets/images/cpu50.png");
    ALLEGRO_BITMAP *conq_gpu50 = al_load_bitmap("../assets/images/gpu50.png");
    ALLEGRO_BITMAP *conq_ram50 = al_load_bitmap("../assets/images/ram50.png");
    ALLEGRO_BITMAP *conq_evolucao1 = al_load_bitmap("../assets/images/evolucao1.png");

    carregarImagensConquistas(conquistas, conq_cpu50, conq_gpu50, conq_ram50, conq_evolucao1);
    // BOTÕES DO MENU
    Botao botoesMenu[3];

    // centralizar
    float escala = 0.4;
    int larguraBotao = al_get_bitmap_width(btn_jogar)*escala;
    int centroX = jogo->larguraTela / 2 - (larguraBotao / 2);
    int yBase = 400;
    int espacamento = 90;

        botoesMenu[0] = (Botao){
        centroX, yBase,
        al_get_bitmap_width(btn_jogar)* escala,
        al_get_bitmap_height(btn_jogar)* escala,
        btn_jogar,
        MENU_JOGAR
    };

        botoesMenu[1] = (Botao){
        centroX, yBase + espacamento,
        al_get_bitmap_width(btn_conquistas)* escala,
        al_get_bitmap_height(btn_conquistas)* escala,
        btn_conquistas,
        MENU_CONQUISTAS
    };

        botoesMenu[2] = (Botao){
        centroX, yBase + espacamento * 2,
        al_get_bitmap_width(btn_sair)* escala,
        al_get_bitmap_height(btn_sair)* escala,
        btn_sair,
        MENU_SAIR
    };

    atribuirImagens(
    slots,
    cpu_img,
    gpu_img,
    ram_img
);

    if (!pc || !menu ||
        !cpu_img || !gpu_img || !ram_img ||
        !pc_evoluido1 || !pc_evoluido2 || !btn_evoluir ||
        !conq_cpu50 || !conq_gpu50 || !conq_ram50 || !conq_evolucao1) {

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
    al_get_keyboard_event_source()
    );
    al_register_event_source(
        queue,
        al_get_display_event_source(display)
    );
   
    al_register_event_source(
    queue,
    al_get_timer_event_source(timerSave)
    );
    al_register_event_source(
            queue,
    al_get_timer_event_source(timerRender)
    );

    al_start_timer(timerSave);
    al_start_timer(timerRender);
    bool rodando = true;
    bool precisa_desenhar = false;

    // LOOP
   while (rodando) {

    al_wait_for_event(queue, &event);

    // ── 1. TECLADO ──────────────────────────────────────────
    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            if (estado == ESTADO_JOGO) {
                estado = ESTADO_MENU;
            } else if (estado == ESTADO_CONQUISTAS) {
                if (conquistaEmZoom >= 0) {
                    conquistaEmZoom = -1;
                } else {
                    estado = ESTADO_MENU;
                }
            }
        }
    }

    // ── 2. FECHAR JANELA ────────────────────────────────────
    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        salvarJogo(jogo, slots, conquistas);
        rodando = false;
    }

    // ── 3. TIMERS ───────────────────────────────────────────
    if (event.type == ALLEGRO_EVENT_TIMER) {
        if (event.timer.source == timerSave) {
            salvarJogo(jogo, slots, conquistas);
            printf("Auto save!\n");
        }
        if (event.timer.source == timerRender) {
            precisa_desenhar = true;

            if (tempoPopup > 0) {
                tempoPopup -= (1.0 / 60.0);
                if (tempoPopup <= 0) {
                    tempoPopup = 0;
                    conquistaParaMostrar = -1;
                }
            }
        }
    }

    // ── 4. MOUSE ────────────────────────────────────────────
    if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
        if (estado == ESTADO_MENU) {
            OpcaoMenu op = tratarCliqueMenu(
                botoesMenu, 3,
                event.mouse.x,
                event.mouse.y
            );
            if (op == MENU_JOGAR) {
                estado = ESTADO_JOGO;
            } else if (op == MENU_CONQUISTAS) {
                estado = ESTADO_CONQUISTAS;
            } else if (op == MENU_SAIR) {
                salvarJogo(jogo, slots, conquistas);
                rodando = false;
            }
        } else if (estado == ESTADO_CONQUISTAS) {

            if (conquistaEmZoom >= 0) {
                // QUALQUER CLIQUE FECHA O ZOOM
                conquistaEmZoom = -1;
            } else {
                int clicado = tratarCliqueConquistas(
                    jogo, conquistas,
                    event.mouse.x, event.mouse.y
                );
                if (clicado >= 0) {
                    conquistaEmZoom = clicado;
                }
            }
        } else if (estado == ESTADO_JOGO) {
            int novaConquista = -1;
        tratarClique(
            jogo,
            slots,
            &historico,
            multiplicador,
            conquistas,
            &novaConquista,
            event.mouse.x,
            event.mouse.y
        );

        if (novaConquista >= 0) {
            conquistaParaMostrar = novaConquista;
            tempoPopup = 4.0; // segundos
        }
        }
    }

    // ── 5. DESENHAR (SEMPRE POR ÚLTIMO) ─────────────────────
    if (precisa_desenhar && rodando) {
        precisa_desenhar = false;

        if (estado == ESTADO_MENU) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_scaled_bitmap(
                bg_menu,
                0, 0,
                al_get_bitmap_width(bg_menu),
                al_get_bitmap_height(bg_menu),
                0, 0,
                jogo->larguraTela,
                jogo->alturaTela,
                0
            );
            desenharMenu(botoesMenu, 3);
            al_flip_display();
        } else if (estado == ESTADO_CONQUISTAS) {
            desenharConquistas(jogo, font, conquistas);
            if (conquistaEmZoom >= 0) {
                desenharZoomConquista(jogo, font, conquistas, conquistaEmZoom);
            }  
        } else if (estado == ESTADO_JOGO) {
        desenhar(
            jogo,
            pc,
            pc_evoluido1,
            pc_evoluido2,
            menu,
            cpu_img,
            gpu_img,
            ram_img,
            btn_evoluir,
            font,
            slots,
            &historico,
            multiplicador,
            largura_original,
            altura_original,
            conquistas,
            conquistaParaMostrar,
            tempoPopup
        );
    }
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
    al_destroy_timer(timerRender);
    al_destroy_display(display);

    free(jogo);

    return 0;
}