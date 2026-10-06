/*
 *
 * Copyright (c) 2023 The ZMK Contributors
 * SPDX-License-Identifier: MIT
 *
 */

#include <zephyr/kernel.h>
#include "util.h"

LV_IMG_DECLARE(bolt);

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    static lv_color_t cbuf_tmp[CANVAS_SIZE * CANVAS_SIZE];
    memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));
    lv_img_dsc_t img;
    img.data = (void *)cbuf_tmp;
    img.header.cf = LV_IMG_CF_TRUE_COLOR;
    img.header.w = CANVAS_SIZE;
    img.header.h = CANVAS_SIZE;

    lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
    lv_canvas_transform(canvas, &img, 900, LV_IMG_ZOOM_NONE, -1, 0, CANVAS_SIZE / 2,
                        CANVAS_SIZE / 2, true);
}

/* Blank panel over everything but the battery row at the top of the screen
 * (the top 20 portrait rows are the right edge of the 160x68 panel).
 */
lv_obj_t *create_idle_cover(lv_obj_t *parent) {
    lv_obj_t *cover = lv_obj_create(parent);
    lv_obj_set_size(cover, 140, 68);
    lv_obj_align(cover, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(cover, LVGL_BACKGROUND, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(cover, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(cover, 0, LV_PART_MAIN);
    lv_obj_set_style_radius(cover, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(cover, 0, LV_PART_MAIN);
    lv_obj_add_flag(cover, LV_OBJ_FLAG_HIDDEN);
    return cover;
}

void set_idle_cover(lv_obj_t *cover, bool idle) {
    if (idle) {
        lv_obj_clear_flag(cover, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(cover, LV_OBJ_FLAG_HIDDEN);
    }
}

void draw_battery(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_black_dsc;
    init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    lv_canvas_draw_rect(canvas, 0, 2, 29, 12, &rect_white_dsc);
    lv_canvas_draw_rect(canvas, 1, 3, 27, 10, &rect_black_dsc);
    lv_canvas_draw_rect(canvas, 2, 4, (state->battery + 2) / 4, 8, &rect_white_dsc);
    lv_canvas_draw_rect(canvas, 30, 5, 3, 6, &rect_white_dsc);
    lv_canvas_draw_rect(canvas, 31, 6, 1, 4, &rect_black_dsc);

    if (state->charging) {
        lv_draw_img_dsc_t img_dsc;
        lv_draw_img_dsc_init(&img_dsc);
        lv_canvas_draw_img(canvas, 9, -1, &bolt, &img_dsc);
    }
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color, const lv_font_t *font,
                    lv_text_align_t align) {
    lv_draw_label_dsc_init(label_dsc);
    label_dsc->color = color;
    label_dsc->font = font;
    label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
    lv_draw_rect_dsc_init(rect_dsc);
    rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color, uint8_t width) {
    lv_draw_line_dsc_init(line_dsc);
    line_dsc->color = color;
    line_dsc->width = width;
}

void init_arc_dsc(lv_draw_arc_dsc_t *arc_dsc, lv_color_t color, uint8_t width) {
    lv_draw_arc_dsc_init(arc_dsc);
    arc_dsc->color = color;
    arc_dsc->width = width;
}
