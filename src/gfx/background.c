#include "gfx.h"

#include <string.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "globals.h"
#include "common.h"

#define CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES 7
#define CONFIG_BACKGROUND_NUM_EXTRA_TILES 7

gfx_background gfx_create_background(gfx_tileset *tileset, size_t *variations, size_t variation_count)
{
    assert((variation_count <= 5));

    gfx_background result;

    result.tileset = tileset;
    memcpy(result.variations, variations, sizeof(size_t) * variation_count);
    result.variation_count = variation_count;

    result._present_variations[0] = variations[rand() % variation_count];
    result._present_variations[1] = variations[rand() % variation_count];
    result._present_variations[2] = variations[rand() % variation_count];

    return result;
}

void gfx_render_background(gfx_background *background)
{
    float tile_width = background->tileset->in_game_width;
    float tile_height = background->tileset->in_game_height;

    /// TODO: do actual batch rendering, this is unoptimal.
    for (int y = 0; y < CONFIG_BACKGROUND_NUM_EXTRA_TILES; y++) {
        for (int x = 0; x < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; x++) {
            int x_pos = tile_width * x - 3 * tile_width;
            int y_pos = tile_height * y;
            gfx_render_tile_camless(background->tileset, 0, 0, (vec2){x_pos, y_pos}, false);
        }
    }
    for (int y = 0; y < CONFIG_BACKGROUND_NUM_EXTRA_TILES; y++) {
        for (int x = 0; x < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; x++) {
            int x_pos = tile_width * x - 3 * tile_width;
            int y_pos = tile_height * y;
            gfx_render_tile_camless(background->tileset, 0, 2, (vec2){x_pos, -y_pos}, false);
        }
    }
    for (int i = 0; i < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; i++) {
        int x = tile_width * i - 3 * tile_width;
        gfx_render_tile_camless(background->tileset, 0, 1, (vec2){x, 0}, false);
    }
}