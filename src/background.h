#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdint.h>
#include <stddef.h>

#include "tileset.h"

typedef struct
{
    tileset *tileset;
} background;

void background_render(background *background);

#endif // BACKGROUND_H