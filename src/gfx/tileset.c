#include "gfx.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "common.h"

gfx_tileset gfx_create_tileset(const char *path, uint8_t x_tile_count, uint8_t y_tile_count)
{
    gfx_tileset result;
    result.x_tile_count = x_tile_count;
    result.y_tile_count = y_tile_count;

    glCreateTextures(GL_TEXTURE_2D, 1, &result.tileset_texture);
    int width, height, num_channels;
    unsigned char *texture = stbi_load(path, &width, &height, &num_channels, 4);
    assert((width > 0 && height > 0 && texture != NULL));
    glTextureStorage2D(result.tileset_texture, 1, GL_RGBA32F, width, height);
    glTextureSubImage2D(result.tileset_texture, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    glTextureParameteri(result.tileset_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(result.tileset_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(result.tileset_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(result.tileset_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTextureParameterfv(result.tileset_texture, GL_TEXTURE_BORDER_COLOR, border_color);

    float width_in_game = floor((float)width / (float)x_tile_count);
    float height_in_game = floor((float)height / (float)y_tile_count);
    result.in_game_width = width_in_game;
    result.in_game_height = height_in_game;

    printf("Tileset: loaded %s, width=%d, height=%d, width_ingame=%lf, height_ingame=%lf\n", path,
           width, height, width_in_game, height_in_game);

    float vertices[] = {
        //   POS         UV
        -width_in_game/2,  height_in_game/2, 0.0f, 0.0f,
         width_in_game/2,  height_in_game/2, 1.0f, 0.0f,
         width_in_game/2, -height_in_game/2, 1.0f, 1.0f,
        -width_in_game/2, -height_in_game/2, 0.0f, 1.0f
    };
    glCreateBuffers(1, &result.vertex_buffer);
    glNamedBufferStorage(result.vertex_buffer, sizeof(vertices), vertices, 0);

    glCreateVertexArrays(1, &result.vertex_array);
    glBindVertexArray(result.vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, result.vertex_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    return result;
}

void gfx_delete_tileset(gfx_tileset *tileset)
{
    /// TODO: Do actual cleanup of all the OpenGL resources
    UNUSED_PARAMETER(tileset);
}