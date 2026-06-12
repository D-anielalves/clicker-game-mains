#include "render.h"
#include "upgrades.h"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

// =====================
// DESENHAR
// =====================
void desenhar(Jogo *jogo,
              ALLEGRO_BITMAP *pc,
              ALLEGRO_BITMAP *menu,
              ALLEGRO_BITMAP *cpu_img,
              ALLEGRO_BITMAP *gpu_img,
              ALLEGRO_BITMAP *ram_img,
              ALLEGRO_FONT *font,
              Slot slots[],
              Pilha *historico,
              int multiplicador[3][1],
              int largura_original,
              int altura_original) {

    // FUNDO
    al_draw_scaled_bitmap(
        menu,
        0, 0,
        al_get_bitmap_width(menu),
        al_get_bitmap_height(menu),
        0, 0,
        jogo->larguraTela,
        jogo->alturaTela,
        0
    );

    // CENTRALIZA PC
    float area_util_x = jogo->larguraTela - 350;

    jogo->img_x = (area_util_x / 2) - (jogo->img_w / 2) + 90;
    jogo->img_y = (jogo->alturaTela / 2) - (jogo->img_h / 2);

    // DESENHA PC
    al_draw_scaled_bitmap(
        pc,
        0, 0,
        largura_original,
        altura_original,
        jogo->img_x,
        jogo->img_y,
        jogo->img_w,
        jogo->img_h,
        0
    );

    int mult = multiplicador[jogo->opcaoCompra][0];

    Slot *recomendado =
    buscarMelhorUpgrade(jogo, slots);

    // HUD
    float centro_pc_x = jogo->img_x + (jogo->img_w / 2) - 180;
    float base_pc_y = jogo->img_y + jogo->img_h - 60;

    al_draw_textf(
        font,
        al_map_rgb(255,255,255),
        centro_pc_x,
        base_pc_y,
        ALLEGRO_ALIGN_CENTER,
        "Pontos: %d",
        jogo->pontos
    );

    al_draw_textf(
        font,
        al_map_rgb(255,255,0),
        centro_pc_x,
        base_pc_y + 30,
        ALLEGRO_ALIGN_CENTER,
        "Modo Compra: %dx",
        mult
    );

    if (recomendado != NULL) {
    al_draw_textf(
        font,
        al_map_rgb(0,255,255),
        centro_pc_x,
        base_pc_y + 60,
        ALLEGRO_ALIGN_CENTER,
        "Melhor Upgrade: %s",
        recomendado->up.nome
        );
    }
    if (historico->topo >= 0) {

    al_draw_textf(
        font,
        al_map_rgb(255,150,0),
        centro_pc_x,
        base_pc_y + 90,
        ALLEGRO_ALIGN_CENTER,
        "Ultima compra: %s",
        historico->itens[historico->topo].nome
         );
    }   


    // PAINEL UPGRADES
    int painel_x = jogo->larguraTela - (BOTAO_W + 90);
    int start_y = 150;

    for (int i = 0; i < NUM_SLOTS; i++) {

        int by = start_y + (i * (BOTAO_H + ESPACAMENTO));

        int custoAtual =
            calcularCustoRecursivo(
                slots[i].up.custo,
                slots[i].nivel
            );

        int custoTotal = custoAtual * mult;

        // IMAGEM
        al_draw_scaled_bitmap(
            slots[i].img,
            0, 0,
            al_get_bitmap_width(slots[i].img),
            al_get_bitmap_height(slots[i].img),
            painel_x,
            by,
            BOTAO_W,
            BOTAO_H,
            0
        );

        // TEXTO
        al_draw_textf(
            font,
            al_map_rgb(255,255,255),
            painel_x + 200,
            by + 35,
            0,
            "%s (Lv %d)",
            slots[i].up.nome,
            slots[i].nivel
        );

        ALLEGRO_COLOR corTexto =
            (jogo->pontos >= custoTotal)
            ? al_map_rgb(0,255,0)
            : al_map_rgb(255,50,50);

        al_draw_textf(
            font,
            corTexto,
            painel_x + 200,
            by + 65,
            0,
            "Custo: $%d",
            custoTotal
        );
    }

    al_flip_display();
}