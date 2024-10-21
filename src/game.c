#include "game.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>
#include <cglm/cglm.h>
#include <GLFW/glfw3.h>

#include "gfx/gfx.h"
#include "globals.h"
#include "level.h"
#include "common.h"

float elapsed = 0.0f;

vec2 camera_position = {0.0, 0.0};

gfx_tileset character_tileset;
gfx_tileset background_tileset;
gfx_tileset level_tileset;

gfx_background level_background;

vec2 player_pos;
gfx_animation player;
float player_horizontal_speed = 500.0f;
float player_vertical_speed = 10.0f;
bool player_fliped = true;

level first_level;

void game_begin()
{
    mat4 proj_matrix;
    /// NOTE: Top/Right has been selected as the background sprite is 270 pixels high and the screen
    ///       is a square. This way is easier to scale the sprites inside the game and avoid mixels.
    glm_ortho(-270/2, 270/2, -270/2, 270/2, 0.1, 100.0f, proj_matrix);

    gfx_initialize(proj_matrix);

    character_tileset = gfx_create_tileset("res/tilesets/characters.png", 9, 3);
    background_tileset = gfx_create_tileset("res/tilesets/backgrounds.png", 8, 3);
    level_tileset = gfx_create_tileset("res/tilesets/scene.png", 20, 9);

    player = (gfx_animation){
        .tileset = &character_tileset,
        .frames = {
            (gfx_animation_frame){.x = 0, .y = 0},
            (gfx_animation_frame){.x = 1, .y = 0}
        },
        .frame_count = 2,
        .seconds_per_frame = 0.25
    };

    size_t variations[] = {0, 1};
    level_background = gfx_create_background(&background_tileset, variations, 2);

    first_level = level_create("res/level.txt", &level_tileset);


    player_pos[0] = 0.0f;
    player_pos[1] = -75.0f;
}

void game_update(float delta_time)
{
    if (glfwGetKey(game_window, GLFW_KEY_A) == GLFW_PRESS) {
        player_pos[0] -= player_horizontal_speed * delta_time;
        player_fliped = false;
    } else if (glfwGetKey(game_window, GLFW_KEY_D) == GLFW_PRESS) {
        player_pos[0] += player_horizontal_speed * delta_time;
        player_fliped = true;
    }

    camera_position[0] = player_pos[0];

    gfx_begin_frame(camera_position);
    gfx_render_background(&level_background);
    level_render(&first_level);
    gfx_render_animation(&player, player_pos, player_fliped);
    gfx_end_frame();
}

void game_end()
{

}