#include "game.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>
#include <stb_image/stb_image.h>

float elapsed = 0.0f;

unsigned int vertex_array;
unsigned int vertex_buffer;
unsigned int shader_program;
unsigned int character_tilemap;

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

void game_begin()
{
    float vertices[] = {
        //   POS         UV
        -0.5f,  0.5f, 0.0f, 0.0f,
         0.5f,  0.5f, 1.0f, 0.0f,
         0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, 0.0f, 1.0f
    };
    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, 0);

    glCreateVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (const void *)(2 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    uint32_t vertex_shader = create_shader(GL_VERTEX_SHADER, "res/shaders/vertex.glsl");
    uint32_t fragment_shader = create_shader(GL_FRAGMENT_SHADER, "res/shaders/fragment.glsl");
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glValidateProgram(shader_program);

    glCreateTextures(GL_TEXTURE_2D, 1, &character_tilemap);
    int width, height, num_channels;
    unsigned char *texture = stbi_load("res/tilemaps/tilemap-characters_packed.png",
                                       &width,
                                       &height,
                                       &num_channels,
                                       4);
    glTextureStorage2D(character_tilemap, 1, GL_RGBA32F, width, height);
    glTextureSubImage2D(character_tilemap, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, texture);
    glTextureParameteri(character_tilemap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(character_tilemap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(character_tilemap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(character_tilemap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTextureParameterfv(character_tilemap, GL_TEXTURE_BORDER_COLOR, border_color);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0);
}

void game_update(float delta_time)
{
    elapsed += delta_time;
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTextureUnit(0, character_tilemap);
    glBindVertexArray(vertex_array);
    glUseProgram(shader_program);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
}

void game_end()
{

}