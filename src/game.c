#include "game.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "tilemap.h"

float elapsed = 0.0f;

tilemap character_tilemap;

void game_begin()
{
    glClearColor(0.0f, 0.0f, 1.0f, 1.0);

    character_tilemap = tilemap_create("res/tilemaps/tilemap-characters_packed.png", 9, 3);
}

void game_update(float delta_time)
{
    elapsed += delta_time;
    glClear(GL_COLOR_BUFFER_BIT);

    tilemap_render(&character_tilemap, 0, 0);
}

void game_end()
{

}