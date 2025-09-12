#include "lvgl.h"
#include "LGFX_S3_RGB.hpp"
#include "lv_draw_gfx.h"
#include "lvgl/src/display/lv_display_private.h"
#include <string.h>

/* LovyanGFX based draw callbacks */

void lv_draw_gfx_fill_cb(lv_draw_ctx_t *draw_ctx, const lv_draw_gfx_fill_dsc_t *dsc, const lv_area_t *area)
{
    LV_UNUSED(draw_ctx);
    if(dsc->opa <= LV_OPA_MIN) return;

    const int32_t w = lv_area_get_width(area);
    const int32_t h = lv_area_get_height(area);
    const uint32_t color = lv_color_to32(dsc->color);

    if(dsc->opa >= LV_OPA_MAX) {
        gfx.fillRect(area->x1, area->y1, w, h, color);
    } else {
        gfx.fillRect(area->x1, area->y1, w, h, color, dsc->opa);
    }
}

void lv_draw_gfx_blend_cb(lv_draw_ctx_t *draw_ctx, const lv_draw_gfx_blend_dsc_t *dsc)
{
    LV_UNUSED(draw_ctx);
    if(dsc->opa <= LV_OPA_MIN) return;

    const lv_area_t *area = dsc->dest_area;
    const int32_t w = lv_area_get_width(area);
    const int32_t h = lv_area_get_height(area);
    const uint16_t *src = (const uint16_t *)dsc->src_buf;

    if(dsc->opa >= LV_OPA_MAX) {
        gfx.pushImageDMA(area->x1, area->y1, w, h, src);
    } else {
        gfx.pushImageDMA(area->x1, area->y1, w, h, src, dsc->opa);
    }
    gfx.waitDMA();
}

void lv_draw_gfx_init(lv_display_t *disp, lv_draw_gfx_t *gfx)
{
    lv_draw_ctx_t *draw_ctx = lv_display_get_draw_ctx(disp);
    LV_ASSERT(draw_ctx);
    LV_ASSERT(gfx);

    memcpy(&gfx->base, draw_ctx, sizeof(lv_draw_ctx_t));
    lv_display_set_draw_ctx(disp, &gfx->base);
}

lv_draw_gfx_t lgfx_draw_ctx = {
    .base = {0},
    .fill_cb = lv_draw_gfx_fill_cb,
    .blend_cb = lv_draw_gfx_blend_cb,
};

