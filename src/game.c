#include "game.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "tilemap.h"
#include "animation.h"

float elapsed = 0.0f;

tilemap character_tilemap;

animation player;

void game_begin()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0);

    character_tilemap = tilemap_create("res/tilemaps/tilemap-characters_packed.png", 9, 3);

    player = (animation){
        .tilemap = &character_tilemap,
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

    animation_render(&player);
}

void game_end()
{

}