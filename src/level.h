#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>

#include <cglm/cglm.h>

#include "gfx/gfx.h"

#define LEVEL_CONFIG_WORLD_POSITION {-130.0, -150.0}

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
    TILE_TYPE_DECORATION2,
    TILE_TYPE_MAX,
} level_tile_type;

typedef struct
{
    level_tile_type *level;
    size_t width;
    size_t height;

    gfx_tileset *tileset;

    vec2 flag_pos;

    size_t num_clouds;
    vec2 cloud_pos[10];
} level;

level level_create(const char *path, gfx_tileset *tileset);

void level_render(level *level);

#endif // LEVEL_H