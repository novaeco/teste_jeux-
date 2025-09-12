#include "ui_sprite.h"

/*
 * s_sprites must be created after the global `gfx` instance is fully
 * constructed.  Static initialization of LGFX_Sprite with `gfx` at file scope
 * leads to an undefined order-of-initialization and results in `gfx` being
 * used before it is ready.  Allocate the sprites at runtime instead.
 */
static LGFX_Sprite *s_sprites[2] = { nullptr, nullptr };
static int s_front = 0;
static int s_back = 1;

void ui_sprite_init(int width, int height) {
    for (int i = 0; i < 2; ++i) {
        if (!s_sprites[i]) {
            s_sprites[i] = new LGFX_Sprite(&gfx);
        }
        s_sprites[i]->createSprite(width, height);
    }
}

LGFX_Sprite *ui_sprite_get_back(void) {
    return s_sprites[s_back];
}

void ui_sprite_push(void) {
    s_sprites[s_back]->pushSprite(0, 0);
    int tmp = s_front;
    s_front = s_back;
    s_back = tmp;
}

