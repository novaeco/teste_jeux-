#pragma once

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    lv_color_t color;
    lv_opa_t opa;
} lv_draw_gfx_fill_dsc_t;

typedef struct {
    const void *src_buf;
    const lv_area_t *dest_area;
    lv_opa_t opa;
} lv_draw_gfx_blend_dsc_t;

typedef void (*lv_draw_gfx_fill_cb_t)(lv_draw_ctx_t *draw_ctx,
                                      const lv_draw_gfx_fill_dsc_t *dsc,
                                      const lv_area_t *area);
typedef void (*lv_draw_gfx_blend_cb_t)(lv_draw_ctx_t *draw_ctx,
                                       const lv_draw_gfx_blend_dsc_t *dsc);

typedef struct {
    lv_draw_ctx_t base;
    lv_draw_gfx_fill_cb_t fill_cb;
    lv_draw_gfx_blend_cb_t blend_cb;
} lv_draw_gfx_t;

void lv_draw_gfx_init(lv_draw_ctx_t *draw_ctx, lv_draw_gfx_t *gfx);
void lv_draw_gfx_fill_cb(lv_draw_ctx_t *draw_ctx, const lv_draw_gfx_fill_dsc_t *dsc, const lv_area_t *area);
void lv_draw_gfx_blend_cb(lv_draw_ctx_t *draw_ctx, const lv_draw_gfx_blend_dsc_t *dsc);

extern lv_draw_gfx_t lgfx_draw_ctx;

#ifdef __cplusplus
}
#endif

