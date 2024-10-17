#include "level.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "globals.h"
#include "common.h"

static level_tile_type map_letter_to_tile_type(char letter)
{
    int decoration_variation = rand() % 2;
    int dirt_variation = rand() % 3;
    switch (letter) {
        case 'a': return TILE_TYPE_AIR;
        case 'g':
            if (dirt_variation == 0)      return TILE_TYPE_GRASS_MIDDLE0;
            else if (dirt_variation == 1) return TILE_TYPE_GRASS_MIDDLE1;
            else                          return TILE_TYPE_GRASS_MIDDLE2;
        case 'G': return TILE_TYPE_GRASS_TOP;
        case 'B': return TILE_TYPE_BLOCK;
        case 'm': return TILE_TYPE_MISTERY_BOX;
        case 'b': return TILE_TYPE_BRICK;
        case 't': return TILE_TYPE_TUBE_BODY;
        case 'T': return TILE_TYPE_TUBE_TOP;
        case 'd': return decoration_variation == 0 ? TILE_TYPE_DECORATION0 : TILE_TYPE_DECORATION1;
        default:
            printf("Level: letter %c found which does not correspond to any tile\n", letter);
            return TILE_TYPE_AIR;
    }
}

static char tile_type_to_character(level_tile_type letter)
{
    switch (letter) {
        case TILE_TYPE_AIR:           return 'a';
        case TILE_TYPE_GRASS_MIDDLE0:
        case TILE_TYPE_GRASS_MIDDLE1:
        case TILE_TYPE_GRASS_MIDDLE2: return 'g';
        case TILE_TYPE_GRASS_TOP:     return 'G';
        case TILE_TYPE_BLOCK:         return 'B';
        case TILE_TYPE_MISTERY_BOX:   return 'm';
        case TILE_TYPE_BRICK:         return 'b';
        case TILE_TYPE_TUBE_BODY:     return 't';
        case TILE_TYPE_TUBE_TOP:      return 't';
        case TILE_TYPE_DECORATION0:
        case TILE_TYPE_DECORATION1:   return 'd';
        default:                      return 'a';
    }
}

static void tile_type_to_index(level_tile_type type, size_t *x, size_t *y)
{
    assert((x != NULL && y != NULL));
    assert((type < TILE_TYPE_MAX));

    #define CASE(type, x_coord, y_coord) case type: *x = x_coord; *y = y_coord; return
    switch(type) {
        CASE(TILE_TYPE_AIR, 0, 8);
        CASE(TILE_TYPE_GRASS_TOP, 2, 1);
        CASE(TILE_TYPE_GRASS_MIDDLE0, 2, 6);
        CASE(TILE_TYPE_GRASS_MIDDLE1, 4, 0);
        CASE(TILE_TYPE_GRASS_MIDDLE2, 5, 0);
        CASE(TILE_TYPE_BLOCK, 7, 2);
        CASE(TILE_TYPE_MISTERY_BOX, 10, 0);
        CASE(TILE_TYPE_MISTERY_BOX_USED, 10, 1);
        CASE(TILE_TYPE_BRICK, 6, 0);
        CASE(TILE_TYPE_TUBE_TOP, 15, 4);
        CASE(TILE_TYPE_TUBE_BODY, 15, 5);
        CASE(TILE_TYPE_DECORATION0, 4, 6);
        CASE(TILE_TYPE_DECORATION1, 5, 6);
        default:
            *x = 0;
            *y = 8;
            return;
    }
    #undef CASE
}

level level_create(const char *path, gfx_tileset *tileset)
{
    FILE *f = fopen(path, "r");
    assert((f != NULL));
    fseek(f, 0, SEEK_END);
    size_t file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *file_data = malloc(file_size);
    fread(file_data, file_size, 1, f);

    /// NOTE: accessing i-1 inside the loop, the loop needs to start from 1.
    size_t map_height = 0;
    size_t map_width = 1; 
    bool first_line = true;
    for (size_t i = 1; i < file_size; i++) {
        assert((i > 0));
        // We have reached the end of the map when there are two or more newlines
        if (file_data[i] == '\n' && file_data[i - 1] == '\n') {
            break;
        }

        if (file_data[i] != '\n' && first_line == true) {
            map_width++;
        } else if (file_data[i] == '\n') {
            first_line = false;
            map_height++;
        }
    }

    /// TODO: avoid allocation and simply parsing function by simply having a preallocated array of
    ///       tiles that we simply populate.
    level result;
    result.height = map_height;
    result.width = map_width;
    result.level = malloc(map_width * map_height * sizeof(level_tile_type));
    result.tileset = tileset;


    size_t i_level = 0;
    for (size_t i = 0; i < file_size; i++) {
        assert((i + 1 < file_size));
        assert((i_level < map_width * map_height));

        if (file_data[i] == '\n' && file_data[i + 1] == '\n') {
            break;
        } else if (file_data[i] == '\n') {
            continue;
        }

        result.level[i_level] = map_letter_to_tile_type(file_data[i]);

        i_level++;
        if (i_level >= map_width * map_height) {
            break;
        }
    }

    printf("Level: Loaded %zux%zu map\n", result.width, result.height);

    return result;
}

void level_render(level *level)
{
    assert((level->tileset->in_game_height == level->tileset->in_game_width));

    /// TODO: do actual batch rendering, this is unoptimal.
    int tile_size = level->tileset->in_game_height;
    for (size_t y =  0; y < level->height; y++) {
        for (size_t x = 0; x < level->width; x++) {
            if (level->level[x + y * level->width] == TILE_TYPE_AIR) {
                continue;
            }

            float x_pos = (float)x;
            float y_pos = (float)(level->height - y);
            vec2 tile_position = {x_pos * tile_size, y_pos * tile_size};
            vec2 world_offset = LEVEL_CONFIG_WORLD_POSITION;
            glm_vec2_add(tile_position, world_offset, tile_position);

            size_t tile_x, tile_y;
            level_tile_type tile_type = level->level[x + y * level->width];
            tile_type_to_index(level->level[x + y * level->width], &tile_x, &tile_y);

            gfx_render_tile(level->tileset, tile_x, tile_y, tile_position, false);
        }
    }
}