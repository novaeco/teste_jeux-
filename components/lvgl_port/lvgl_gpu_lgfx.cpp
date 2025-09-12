#include "lvgl.h"
#include "LGFX_S3_RGB.hpp"

/* LovyanGFX based draw callbacks */

static void lv_draw_gfx_fill_cb(lv_draw_ctx_t *draw_ctx, const lv_draw_gfx_fill_dsc_t *dsc, const lv_area_t *area)
{
    LV_UNUSED(draw_ctx);
    if(dsc->opa <= LV_OPA_MIN) return;

    const int32_t w = lv_area_get_width(area);
    const int32_t h = lv_area_get_height(area);
    const uint32_t color = lv_color_to32(dsc->color);

    if(dsc->opa >= LV_OPA_MAX) {
        lgfx.fillRect(area->x1, area->y1, w, h, color);
    } else {
        lgfx.fillRect(area->x1, area->y1, w, h, color, dsc->opa);
    }
}

static void lv_draw_gfx_blend_cb(lv_draw_ctx_t *draw_ctx, const lv_draw_gfx_blend_dsc_t *dsc)
{
    LV_UNUSED(draw_ctx);
    if(dsc->opa <= LV_OPA_MIN) return;

    const lv_area_t *area = dsc->dest_area;
    const int32_t w = lv_area_get_width(area);
    const int32_t h = lv_area_get_height(area);
    const uint16_t *src = (const uint16_t *)dsc->src_buf;

    if(dsc->opa >= LV_OPA_MAX) {
        lgfx.pushImageDMA(area->x1, area->y1, w, h, src);
    } else {
        lgfx.pushImageDMA(area->x1, area->y1, w, h, src, dsc->opa);
    }
    lgfx.waitDMA();
}

lv_draw_gfx_t lgfx_draw_ctx = {
    .base = {0},
    .fill_cb = lv_draw_gfx_fill_cb,
    .blend_cb = lv_draw_gfx_blend_cb,
};

