#include "tileset.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "common.h"

static uint32_t create_shader(int type, char *file)
{
    FILE *f = fopen(file, "r");
    assert((f != NULL));
    fseek(f, 0, SEEK_END);
    uint32_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *source = calloc(size, 1);
    fread(source, size, 1, f);

    uint32_t shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    fclose(f);
    free(source);
    return shader;
}

tileset tileset_create(const char *path, uint8_t x_tile_count, uint8_t y_tile_count)
{
    tileset result;
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

    uint32_t vertex_shader = create_shader(GL_VERTEX_SHADER, "res/shaders/tileset.vert.glsl");
    uint32_t fragment_shader = create_shader(GL_FRAGMENT_SHADER, "res/shaders/tileset.frag.glsl");
    result.shader_program = glCreateProgram();
    glAttachShader(result.shader_program, vertex_shader);
    glAttachShader(result.shader_program, fragment_shader);
    glLinkProgram(result.shader_program);
    glValidateProgram(result.shader_program);

    return result;
}

void tileset_delete(tileset *tileset)
{
    /// TODO: Do actual cleanup of all the OpenGL resources
    UNUSED_PARAMETER(tileset);
}

void tileset_render(tileset *tileset, uint8_t tile_x, uint8_t tile_y, mat4 mvp, bool h_flip)
{
    glBindTextureUnit(0, tileset->tileset_texture);
    glBindVertexArray(tileset->vertex_array);
    glUseProgram(tileset->shader_program);

    int size_location = glGetUniformLocation(tileset->shader_program, "u_tileset_size");
    int render_location = glGetUniformLocation(tileset->shader_program, "u_tileset_render");
    assert((size_location != -1 && render_location != -1));
    glUniform2f(size_location, (float)tileset->x_tile_count, (float)tileset->y_tile_count);
    glUniform2f(render_location, (float)tile_x, (float)tile_y);

    int mvp_location = glGetUniformLocation(tileset->shader_program, "u_mvp");
    assert((mvp_location != -1));
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp[0]);

    int h_flip_location = glGetUniformLocation(tileset->shader_program, "u_horizontal_flip");
    assert((h_flip_location != -1));
    glUniform1i(h_flip_location, h_flip);

    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}