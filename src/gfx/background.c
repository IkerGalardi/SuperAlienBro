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

    /// TODO: make this static allocation instead of a malloc
    result.rendered_variations = malloc(CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES * sizeof(size_t));
    for (int i = 0; i < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; i++) {
        result.rendered_variations[i] = rand() % variation_count;
    }

    return result;
}

void gfx_render_background(gfx_background *background)
{
    float tile_width = background->tileset->in_game_width;
    float tile_height = background->tileset->in_game_height;

    /// TODO: do actual batch rendering, this is unoptimal.
    for (int y = 0; y < CONFIG_BACKGROUND_NUM_EXTRA_TILES; y++) {
        for (int x = 0; x < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; x++) {
            int x_pos = tile_width * x - 6 * tile_width;
            int y_pos = tile_height * y;
            size_t variation = background->rendered_variations[x];
            gfx_render_tile_cam_fraction(background->tileset,
                                         variation, 0,
                                         (vec2){x_pos, y_pos},
                                         false,
                                         0.2);
        }
    }
    for (int y = 0; y < CONFIG_BACKGROUND_NUM_EXTRA_TILES; y++) {
        for (int x = 0; x < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; x++) {
            int x_pos = tile_width * x - 6 * tile_width;
            int y_pos = tile_height * y;
            size_t variation = background->rendered_variations[x];
            gfx_render_tile_cam_fraction(background->tileset,
                                         variation, 2,
                                         (vec2){x_pos, -y_pos},
                                         false,
                                         0.2);
        }
    }
    for (int i = 0; i < CONFIG_BACKGROUND_NUM_HORIZONTAL_TILES; i++) {
        int x = tile_width * i - 6 * tile_width;
            size_t variation = background->rendered_variations[i];
            gfx_render_tile_cam_fraction(background->tileset,
                                         variation, 1,
                                         (vec2){x, 0},
                                         false,
                                         0.2);
    }
}