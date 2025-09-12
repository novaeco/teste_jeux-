#include "ui_sprite.h"

static LGFX_Sprite s_sprites[2] = {
    LGFX_Sprite(&lgfx),
    LGFX_Sprite(&lgfx)
};
static int s_front = 0;
static int s_back = 1;

void ui_sprite_init(int width, int height) {
    s_sprites[0].createSprite(width, height);
    s_sprites[1].createSprite(width, height);
}

LGFX_Sprite *ui_sprite_get_back(void) {
    return &s_sprites[s_back];
}

void ui_sprite_push(void) {
    s_sprites[s_back].pushSprite(0, 0);
    int tmp = s_front;
    s_front = s_back;
    s_back = tmp;
}

