#include "level.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static level_tile_type map_letter_to_tile_type(char letter)
{
    int variation = rand() % 2;
    switch (letter) {
        case 'a': return TILE_TYPE_AIR;
        case 'g': return TILE_TYPE_GRASS_MIDDLE;
        case 'G': return TILE_TYPE_GRASS_TOP;
        case 'B': return TILE_TYPE_BLOCK;
        case 'm': return TILE_TYPE_MISTERY_BOX;
        case 'b': return TILE_TYPE_BRICK;
        case 't': return TILE_TYPE_TUBE_BODY;
        case 'T': return TILE_TYPE_TUBE_TOP;
        case 'd': return variation == 0 ? TILE_TYPE_DECORATION0 : TILE_TYPE_DECORATION1;
        default:
            printf("Level: letter %c found which does not correspond to any tile\n", letter);
            return TILE_TYPE_AIR;
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
        CASE(TILE_TYPE_GRASS_MIDDLE, 2, 6);
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

level level_create(const char *path)
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

    return result;
}