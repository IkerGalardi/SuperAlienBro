#include "background.h"

#include <string.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "globals.h"
#include "common.h"

background background_create(tileset *tileset, size_t *variations, size_t variation_count)
{
    background result;

    result.tileset = tileset;
    memcpy(result.variations, variations, sizeof(size_t) * variation_count);
    result.variation_count = variation_count;

    result._present_variations[0] = variations[rand() % variation_count];
    result._present_variations[1] = variations[rand() % variation_count];
    result._present_variations[2] = variations[rand() % variation_count];

    return result;
}

void background_render(background *background)
{
    float tile_width = background->tileset->in_game_width;
    float tile_height = background->tileset->in_game_height;

    /// TODO: do actual batch rendering, this is unoptimal.
    mat4 mvp;
    for (int y = 0; y < CONFIG_BACKGROUND_NUM_EXTRA_TILES; y++) {
        for (int x = 0; x < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; x++) {
            int x_pos = tile_width * x - 3 * tile_width;
            int y_pos = tile_height * y;
            calculate_mvp((vec3){x_pos, y_pos, 0.0}, mvp);
            tileset_render(background->tileset, 0, 0, mvp);
        }
    }
    for (int y = 0; y < CONFIG_BACKGROUND_NUM_EXTRA_TILES; y++) {
        for (int x = 0; x < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; x++) {
            int x_pos = tile_width * x - 3 * tile_width;
            int y_pos = tile_height * y;
            calculate_mvp((vec3){x_pos, -y_pos, 0.0}, mvp);
            tileset_render(background->tileset, 0, 2, mvp);
        }
    }
    for (int i = 0; i < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; i++) {
        int x = tile_width * i - 3 * tile_width;
        calculate_mvp((vec3){x, 0.0, 0.0}, mvp);
        tileset_render(background->tileset, 0, 1, mvp);
    }
}