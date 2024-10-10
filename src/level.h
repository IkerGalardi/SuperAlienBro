#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>

#include "tileset.h"

typedef enum
{
    TILE_TYPE_AIR,
    TILE_TYPE_GRASS_TOP,
    TILE_TYPE_GRASS_MIDDLE,
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
} level;

level level_create(const char *path);

#endif // LEVEL_H