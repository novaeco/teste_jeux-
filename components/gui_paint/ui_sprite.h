#pragma once
#include "LGFX_S3_RGB.hpp"

#ifdef __cplusplus
extern "C" {
#endif

void ui_sprite_init(int width, int height);
LGFX_Sprite *ui_sprite_get_back(void);
void ui_sprite_push(void);

#ifdef __cplusplus
}
#endif

