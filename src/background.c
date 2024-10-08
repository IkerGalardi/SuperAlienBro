#include "background.h"

#include <cglm/cglm.h>

#include "globals.h"

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