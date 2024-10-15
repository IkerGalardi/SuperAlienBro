#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>

#include "tileset.h"

#define LEVEL_CONFIG_WORLD_POSITION {-130.0, -150.0, 0.0}

typedef enum
{
    TILE_TYPE_AIR,
    TILE_TYPE_GRASS_TOP,
    TILE_TYPE_GRASS_MIDDLE0,
    TILE_TYPE_GRASS_MIDDLE1,
    TILE_TYPE_GRASS_MIDDLE2,
    TILE_TYPE_BLOCK,
    TILE_TYPE_MISTERY_BOX,
    TILE_TYPE_MISTERY_BOX_USED,
    TILE_TYPE_BRICK,
    TILE_TYPE_TUBE_TOP,
    TILE_TYPE_TUBE_BODY,
    TILE_TYPE_DECORATION0,
    TILE_TYPE_DECORATION1,
    TILE_TYPE_MAX,
} level_tile_type;

typedef struct
{
    level_tile_type *level;
    size_t width;
    size_t height;

    tileset *tileset;
} level;

level level_create(const char *path, tileset *tileset);

void level_render(level *level);

#endif // LEVEL_H