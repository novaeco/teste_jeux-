#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GAME_MODE_REAL = 0,
    GAME_MODE_SIMULATION
} game_mode_t;

extern game_mode_t g_game_mode;

#ifdef __cplusplus
}
#endif

