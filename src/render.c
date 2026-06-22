#include "render.h"
#include "upgrades.h"
#include "conquistas.h"

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

// =====================
// DESENHAR
// =====================
void desenhar(Jogo *jogo,
              ALLEGRO_BITMAP *pc,
              ALLEGRO_BITMAP *pc_evoluido1,
              ALLEGRO_BITMAP *pc_evoluido2,
              ALLEGRO_BITMAP *menu,
              ALLEGRO_BITMAP *cpu_img,
              ALLEGRO_BITMAP *gpu_img,
              ALLEGRO_BITMAP *ram_img,
              ALLEGRO_BITMAP *btn_evoluir,
              ALLEGRO_FONT *font,
              Slot slots[],
              Pilha *historico,
              int multiplicador[3][1],
              int largura_original,
              int altura_original,
              Conquista conquistas[],
              int conquistaParaMostrar,
              float tempoPopup) {

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

    jogo->img_x = (area_util_x / 2) - (jogo->img_w / 2);
    jogo->img_y = (jogo->alturaTela / 2) - (jogo->img_h / 2);

    // DESENHA PC (escolhe a imagem de acordo com a evolução, mantendo proporção)
    ALLEGRO_BITMAP *pc_atual = pc;
    if (jogo->evolucaoPc == 1) pc_atual = pc_evoluido1;
    if (jogo->evolucaoPc == 2) pc_atual = pc_evoluido2;

    int largura_pc_atual = al_get_bitmap_width(pc_atual);
    int altura_pc_atual = al_get_bitmap_height(pc_atual);

    float escala_w = (float)jogo->img_w / largura_pc_atual;
    float escala_h = (float)jogo->img_h / altura_pc_atual;
    float escala_final = (escala_w < escala_h) ? escala_w : escala_h;

    int img_w_ajustado = largura_pc_atual * escala_final;
    int img_h_ajustado = altura_pc_atual * escala_final;

    float centro_x_original = jogo->img_x + (jogo->img_w / 2);
    float centro_y_original = jogo->img_y + (jogo->img_h / 2);

    // OFFSET INDEPENDENTE POR ESTÁGIO DE EVOLUÇÃO
    int offset_x = 0;
    int offset_y = 0;

    if (jogo->evolucaoPc == 0) { offset_x = +90; offset_y = 0; }
    if (jogo->evolucaoPc == 1) { offset_x = -50;   offset_y = 0; }
    if (jogo->evolucaoPc == 2) { offset_x = -60;   offset_y = 0; }

    int img_x_ajustado = centro_x_original - (img_w_ajustado / 2) + offset_x;
    int img_y_ajustado = centro_y_original - (img_h_ajustado / 2) + offset_y;

    al_draw_scaled_bitmap(
        pc_atual,
        0, 0,
        largura_pc_atual,
        altura_pc_atual,
        img_x_ajustado,
        img_y_ajustado,
        img_w_ajustado,
        img_h_ajustado,
        0
    );

    int mult = multiplicador[jogo->opcaoCompra][0];

    Slot *recomendado =
    buscarMelhorUpgrade(jogo, slots);

    // HUD
    float centro_pc_x = (jogo->larguraTela - 350) / 2 - 80;
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
            painel_x + 150,
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
            painel_x + 150,
            by + 65,
            0,
            "Custo: $%d",
            custoTotal
        );
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

    if (podeEvoluir1 || podeEvoluir2) {
        al_draw_scaled_bitmap(
            btn_evoluir,
            0, 0,
            al_get_bitmap_width(btn_evoluir),
            al_get_bitmap_height(btn_evoluir),
            botaoEvoluir_x,
            botaoEvoluir_y,
            botaoEvoluir_w,
            botaoEvoluir_h,
            0
        );
    }

    // POPUP DE CONQUISTA DESBLOQUEADA
    if (conquistaParaMostrar >= 0 && tempoPopup > 0) {

        ALLEGRO_BITMAP *img_popup = conquistas[conquistaParaMostrar].img;

        int popup_w = 500;
        int popup_h = 120;
        int popup_x = (jogo->larguraTela / 2) - (popup_w / 2);
        int popup_y = 30;

        al_draw_scaled_bitmap(
            img_popup,
            0, 0,
            al_get_bitmap_width(img_popup),
            al_get_bitmap_height(img_popup),
            popup_x,
            popup_y,
            popup_w,
            popup_h,
            0
        );
    }

    al_flip_display();
}
// =====================
// DESENHAR TELA DE CONQUISTAS
// =====================
void desenharConquistas(Jogo *jogo, ALLEGRO_FONT *font, Conquista conquistas[]) {

    int painel_x = 60;
    int painel_y = 30;
    int painel_w = jogo->larguraTela - 120;
    int painel_h = jogo->alturaTela - 60;

    // FUNDO ESCURO GERAL
    al_clear_to_color(al_map_rgb(10, 12, 20));

    // PAINEL PRINCIPAL (azul escuro)
    al_draw_filled_rectangle(
        painel_x, painel_y,
        painel_x + painel_w, painel_y + painel_h,
        al_map_rgb(20, 30, 55)
    );

    // BRILHO RADIAL SIMULADO (círculos translúcidos concêntricos no centro)
    int centro_x = painel_x + painel_w / 2;
    int centro_y = painel_y + painel_h / 2;
    for (int r = 250; r > 0; r -= 25) {
        unsigned char alpha = (unsigned char)(60 - r * 0.2);
        al_draw_filled_circle(
            centro_x, centro_y, r,
            al_map_rgba(60, 110, 180, alpha > 0 ? alpha : 5)
        );
    }

    // BORDA EXTERNA (estilo retro, grossa)
    al_draw_rectangle(
        painel_x, painel_y,
        painel_x + painel_w, painel_y + painel_h,
        al_map_rgb(120, 170, 230),
        4
    );

    // FAIXA DO TÍTULO (barra superior mais clara)
    int faixa_h = 70;
    al_draw_filled_rectangle(
        painel_x, painel_y,
        painel_x + painel_w, painel_y + faixa_h,
        al_map_rgb(70, 130, 210)
    );
    al_draw_line(
        painel_x, painel_y + faixa_h,
        painel_x + painel_w, painel_y + faixa_h,
        al_map_rgb(120, 170, 230), 4
    );

    // "CONECTORES" NOS CANTOS DA FAIXA (linhas tipo circuito)
    al_draw_line(painel_x + 20, painel_y + 20, painel_x + 80, painel_y + 20, al_map_rgb(150, 200, 255), 3);
    al_draw_line(painel_x + 80, painel_y + 20, painel_x + 80, painel_y + 40, al_map_rgb(150, 200, 255), 3);
    al_draw_filled_circle(painel_x + 20, painel_y + 20, 4, al_map_rgb(180, 220, 255));

    al_draw_line(painel_x + painel_w - 20, painel_y + 20, painel_x + painel_w - 80, painel_y + 20, al_map_rgb(150, 200, 255), 3);
    al_draw_line(painel_x + painel_w - 80, painel_y + 20, painel_x + painel_w - 80, painel_y + 40, al_map_rgb(150, 200, 255), 3);
    al_draw_filled_circle(painel_x + painel_w - 20, painel_y + 20, 4, al_map_rgb(180, 220, 255));

    // TÍTULO
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        jogo->larguraTela / 2,
        painel_y + 22,
        ALLEGRO_ALIGN_CENTRE,
        "CONQUISTAS"
    );

    // LISTA DE CARDS
    int card_x = painel_x + 30;
    int card_w = painel_w - 60;
    int card_h = 95;
    int espacamento = 16;
    int start_y = painel_y + faixa_h + 20;

    for (int i = 0; i < NUM_CONQUISTAS; i++) {

        int card_y = start_y + i * (card_h + espacamento);

        // FUNDO DO CARD
        al_draw_filled_rectangle(
            card_x, card_y,
            card_x + card_w, card_y + card_h,
            al_map_rgba(15, 20, 35, 230)
        );
        al_draw_rectangle(
            card_x, card_y,
            card_x + card_w, card_y + card_h,
            al_map_rgb(80, 110, 160), 2
        );

        // IMAGEM (escurecida se bloqueada)
        int img_size = card_h - 16;
        ALLEGRO_COLOR tint = conquistas[i].desbloqueada
            ? al_map_rgb(255, 255, 255)
            : al_map_rgb(35, 35, 40);

        al_draw_tinted_scaled_bitmap(
            conquistas[i].img,
            tint,
            0, 0,
            al_get_bitmap_width(conquistas[i].img),
            al_get_bitmap_height(conquistas[i].img),
            card_x + 8,
            card_y + 8,
            img_size,
            img_size,
            0
        );

        // NOME (sempre visível)
        al_draw_text(
            font,
            al_map_rgb(255, 255, 255),
            card_x + img_size + 30,
            card_y + 20,
            0,
            conquistas[i].nome
        );

        // DESCRIÇÃO (ou "???" se bloqueada)
        al_draw_text(
            font,
            conquistas[i].desbloqueada
                ? al_map_rgb(100, 255, 170)
                : al_map_rgb(120, 120, 130),
            card_x + img_size + 30,
            card_y + 50,
            0,
            conquistas[i].desbloqueada
                ? conquistas[i].descricao
                : "???"
        );
    }

    // RODAPÉ
    al_draw_text(
        font,
        al_map_rgb(180, 190, 210),
        jogo->larguraTela / 2,
        painel_y + painel_h - 35,
        ALLEGRO_ALIGN_CENTRE,
        "ESC para voltar"
    );

    al_flip_display();
}
// =====================
// DESENHAR ZOOM DE CONQUISTA
// =====================
void desenharZoomConquista(Jogo *jogo, ALLEGRO_FONT *font, Conquista conquistas[], int indice) {

    // FUNDO ESCURECIDO POR CIMA DE TUDO
    al_draw_filled_rectangle(
        0, 0,
        jogo->larguraTela, jogo->alturaTela,
        al_map_rgba(0, 0, 0, 200)
    );

    ALLEGRO_BITMAP *img = conquistas[indice].img;

    int largura_original_img = al_get_bitmap_width(img);
    int altura_original_img = al_get_bitmap_height(img);

    // TAMANHO MÁXIMO DISPONÍVEL (mantendo a proporção original)
    int max_w = 500;
    int max_h = 350;

    float escala_w = (float)max_w / largura_original_img;
    float escala_h = (float)max_h / altura_original_img;
    float escala_zoom = (escala_w < escala_h) ? escala_w : escala_h;

    int img_w_zoom = largura_original_img * escala_zoom;
    int img_h_zoom = altura_original_img * escala_zoom;

    int img_x = (jogo->larguraTela / 2) - (img_w_zoom / 2);
    int img_y = 100;

    // BRILHO POR TRÁS DA IMAGEM (destaque)
    al_draw_filled_circle(
        jogo->larguraTela / 2,
        img_y + img_h_zoom / 2,
        (img_w_zoom > img_h_zoom ? img_w_zoom : img_h_zoom) / 2 + 30,
        al_map_rgba(80, 150, 220, 60)
    );

    al_draw_scaled_bitmap(
        img,
        0, 0,
        largura_original_img,
        altura_original_img,
        img_x, img_y,
        img_w_zoom, img_h_zoom,
        0
    );

    // BORDA EM TORNO DA IMAGEM
    al_draw_rectangle(
        img_x, img_y,
        img_x + img_w_zoom, img_y + img_h_zoom,
        al_map_rgb(150, 200, 255), 3
    );

    // NOME
    al_draw_text(
        font,
        al_map_rgb(255, 255, 255),
        jogo->larguraTela / 2,
        img_y + img_h_zoom + 30,
        ALLEGRO_ALIGN_CENTRE,
        conquistas[indice].nome
    );

    // DESCRIÇÃO
    al_draw_text(
        font,
        al_map_rgb(100, 255, 170),
        jogo->larguraTela / 2,
        img_y + img_h_zoom + 65,
        ALLEGRO_ALIGN_CENTRE,
        conquistas[indice].descricao
    );

    // DICA DE FECHAR
    al_draw_text(
        font,
        al_map_rgb(180, 190, 210),
        jogo->larguraTela / 2,
        jogo->alturaTela - 50,
        ALLEGRO_ALIGN_CENTRE,
        "Clique ou ESC para fechar"
    );

    al_flip_display();
}