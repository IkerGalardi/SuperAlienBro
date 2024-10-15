#include "game.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#include "tileset.h"
#include "animation.h"
#include "globals.h"
#include "background.h"
#include "level.h"
#include "common.h"

float elapsed = 0.0f;

tileset character_tileset;
tileset background_tileset;
tileset level_tileset;

background level_background;

vec3 player_pos;
animation player;
float player_horizontal_speed = 500.0f;
float player_vertical_speed = 10.0f;

level first_level;

void game_begin()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0);

    character_tileset = tileset_create("res/tilesets/characters.png", 9, 3);
    background_tileset = tileset_create("res/tilesets/backgrounds.png", 4, 3);
    level_tileset = tileset_create("res/tilesets/scene.png", 20, 9);

    player = (animation){
        .tileset = &character_tileset,
        .frames = {
            (animation_frame){.x = 0, .y = 0},
            (animation_frame){.x = 1, .y = 0}
        },
        .frame_count = 2,
        .seconds_per_frame = 0.25
    };

    size_t variations[] = {0, 1};
    level_background = background_create(&background_tileset, variations, 2);

    first_level = level_create("res/level.txt", &level_tileset);

    /// NOTE: Top/Right has been selected as the background sprite is 270 pixels high and the screen
    ///       is a square. This way is easier to scale the sprites inside the game and avoid mixels.
    glm_ortho(-270/2, 270/2, -270/2, 270/2, 0.1, 100.0f, projection_matrix);

    player_pos[0] = 0.0f;
    player_pos[1] = -75.0f;
    player_pos[3] = 0.0f;
}

void game_update(float delta_time)
{
    glClear(GL_COLOR_BUFFER_BIT);

    background_render(&level_background);
    level_render(&first_level);

    if (glfwGetKey(game_window, GLFW_KEY_A) == GLFW_PRESS) {
        player_pos[0] -= player_horizontal_speed * delta_time;
    } else if (glfwGetKey(game_window, GLFW_KEY_D) == GLFW_PRESS) {
        player_pos[0] += player_horizontal_speed * delta_time;
    }

    mat4 mvp;
    calculate_mvp(player_pos, mvp);
    animation_render(&player, mvp);

    camera_position[0] = player_pos[0];
}

void game_end()
{

}