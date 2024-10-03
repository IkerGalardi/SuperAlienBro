#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include <stddef.h>

#include "tileset.h"

typedef struct
{
    uint8_t x, y;
} animation_frame;

typedef struct
{
    tileset *tileset;

    animation_frame frames[5];
    size_t frame_count;

    double seconds_per_frame;
} animation;

void animation_render(animation *animation, mat4 mvp);

#endif // ANIMATION_H