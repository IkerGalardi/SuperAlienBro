#include "game.h"

#include <math.h>

#include <glad/glad.h>

float elapsed = 0.0f;

void game_begin()
{

}

void game_update(float delta_time)
{
    elapsed += delta_time * 30;
    float red = sinf(elapsed);
    float green = sinf(elapsed + 10);
    float blue = sinf(elapsed + 20);

    glClearColor(red, green, blue, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void game_end()
{

}