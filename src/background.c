#include "background.h"

#include <string.h>
#include <stdlib.h>

#include <cglm/cglm.h>

#include "globals.h"

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

    // Render left tile
    mat4 translation;
    glm_mat4_identity(translation);
    glm_translate(translation, (vec3){-tile_width, 0.0, 0.0});
    mat4 mvp;
    glm_mul(projection_matrix, translation, mvp);
    tileset_render(background->tileset, 0, 0, mvp);

    // Render center tile
    glm_mat4_identity(translation);
    glm_translate(translation, (vec3){0.0, 0.0, 0.0});
    glm_mul(projection_matrix, translation, mvp);
    tileset_render(background->tileset, 0, 0, mvp);

    // Render right tile
    glm_mat4_identity(translation);
    glm_translate(translation, (vec3){tile_width, 0.0, 0.0});
    glm_mul(projection_matrix, translation, mvp);
    tileset_render(background->tileset, 0, 0, mvp);
}