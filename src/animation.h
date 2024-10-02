#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include <stddef.h>

#include "tilemap.h"

typedef struct
{
    uint8_t x, y;
} animation_frame;

typedef struct
{
    tilemap *tilemap;

    animation_frame frames[5];
    size_t frame_count;

    double seconds_per_frame;
} animation;

void animation_render(animation *animation);

#endif // ANIMATION_H