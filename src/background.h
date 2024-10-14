#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdint.h>
#include <stddef.h>

#include "tileset.h"

#define CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES 7
#define CONFIG_BACKGROUND_NUM_EXTRA_TILES 7

typedef struct
{
    tileset *tileset;

    size_t variations[5];
    size_t variation_count;

    size_t _present_variations[3];
} background;

background background_create(tileset *tileset, size_t *variations, size_t variation_count);

void background_render(background *background);

#endif // BACKGROUND_H