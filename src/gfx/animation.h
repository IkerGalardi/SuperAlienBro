#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include <stddef.h>

#include "tileset.h"

typedef struct
{
    uint8_t x, y;
} gfx_animation_frame;

typedef struct
{
    gfx_tileset *tileset;

    gfx_animation_frame frames[5];
    size_t frame_count;

    double seconds_per_frame;
} gfx_animation;

void gfx_render_animation(gfx_animation *animation, mat4 mvp, bool h_flip);

#endif // ANIMATION_H