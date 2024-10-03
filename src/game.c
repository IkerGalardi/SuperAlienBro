#include "game.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <cglm/cglm.h>

#include "tileset.h"
#include "animation.h"

float elapsed = 0.0f;

tileset character_tileset;

animation player;

void game_begin()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0);

    character_tileset = tileset_create("res/tilesets/characters.png", 9, 3);

    player = (animation){
        .tileset = &character_tileset,
        .frames = {
            (animation_frame){.x = 0, .y = 0},
            (animation_frame){.x = 1, .y = 0}
        },
        .frame_count = 2,
        .seconds_per_frame = 0.25
    };
}

void game_update(float delta_time)
{
    glClear(GL_COLOR_BUFFER_BIT);

    /// NOTE: Top/Right has been selected as the background sprite is 72 pixels high and the screen
    ///       is a square. This way is easier to scale the sprites inside the game and avoid mixels.
    mat4 proj_matrix;
    glm_ortho(-72/2, 72/2, -72/2, 72/2, 0.1, 100.0f, proj_matrix);

    animation_render(&player, proj_matrix);
}

void game_end()
{

}