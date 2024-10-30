#include "gfx.h"

#include <glad/glad.h>

#define RENDERER_CONFIG_QUADS_PER_BATCH 100000

#define RENDERER_INDEX_BUFFER_COUNT RENDERER_CONFIG_QUADS_PER_BATCH * 6
#define RENDERER_VERTEX_BUFFER_SIZE RENDERER_CONFIG_QUADS_PER_BATCH * 4 * sizeof(vertex)

typedef struct
{
    vec2 pos;
    vec2 uv;
} vertex;

static bool initialized = false;
static bool frame_began = false;

static mat4 projection_matrix;
static vec2 camera_pos;

static uint32_t shader_program;

static uint32_t vertex_array;
static uint32_t vertex_buffer;
static uint32_t index_buffer;

size_t buffer_offset = 0;
size_t num_indices = 0;

static void calc_mvp(vec2 pos, mat4 out)
{
    // Translate to the object position
    vec3 pos3 = {pos[0], pos[1], 0.0};
    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, pos3);

    // Negativelly translate by the camera position
    vec3 camera_pos3 = {camera_pos[0], camera_pos[1], 0.0};
    vec3 negative_camera_pos;
    glm_vec3_copy(camera_pos3, negative_camera_pos);
    negative_camera_pos[0] = -negative_camera_pos[0];
    negative_camera_pos[1] = -negative_camera_pos[1];
    negative_camera_pos[2] = -negative_camera_pos[2];
    glm_translate(transform, negative_camera_pos);

    glm_mul(projection_matrix, transform, out);
}

static void calc_mvp_cam_fraction(vec2 pos, float fraction, mat4 out)
{
    // Translate to the object position
    vec3 pos3 = {pos[0], pos[1], 0.0};
    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, pos3);

    // Negativelly translate by the camera position
    vec3 camera_pos3 = {camera_pos[0] * fraction, camera_pos[1] * fraction, 0.0};
    vec3 negative_camera_pos;
    glm_vec3_copy(camera_pos3, negative_camera_pos);
    negative_camera_pos[0] = -negative_camera_pos[0];
    negative_camera_pos[1] = -negative_camera_pos[1];
    negative_camera_pos[2] = -negative_camera_pos[2];
    glm_translate(transform, negative_camera_pos);

    glm_mul(projection_matrix, transform, out);
}

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

void gfx_initialize(mat4 proj_matrix)
{
    assert((initialized == false));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 1.0f, 1.0);

    uint32_t vertex_shader = create_shader(GL_VERTEX_SHADER, "res/shaders/batch.vert.glsl");
    uint32_t fragment_shader = create_shader(GL_FRAGMENT_SHADER, "res/shaders/batch.frag.glsl");
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glValidateProgram(shader_program);

    // This module is the only one in charge of changing the OpenGL global state. No need
    // to later
    glUseProgram(shader_program);

    glCreateBuffers(1, &vertex_buffer);
    glNamedBufferData(vertex_buffer, RENDERER_VERTEX_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

    uint32_t *indices = malloc(RENDERER_INDEX_BUFFER_COUNT * sizeof(uint32_t));
    size_t offset = 0;
    for(size_t i = 0; i < RENDERER_INDEX_BUFFER_COUNT; i += 6) {
        assert((i + 5 < RENDERER_INDEX_BUFFER_COUNT));

        indices[i + 0] = offset + 3;
        indices[i + 1] = offset + 0;
        indices[i + 2] = offset + 1;

        indices[i + 3] = offset + 1;
        indices[i + 4] = offset + 2;
        indices[i + 5] = offset + 3;

        offset += 4;
    }

    glCreateBuffers(1, &index_buffer);
    glNamedBufferStorage(index_buffer,
                         RENDERER_INDEX_BUFFER_COUNT * sizeof(uint32_t),
                         indices,
                         0);

    glCreateVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), offsetof(vertex, pos));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), offsetof(vertex, uv));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glm_mat4_copy(proj_matrix, projection_matrix);

    initialized = true;
}

void gfx_begin_frame(vec2 camera_position)
{
    assert((initialized == true));
    assert((frame_began == false));

    glClear(GL_COLOR_BUFFER_BIT);

    glm_vec2_copy(camera_position, camera_pos);

    num_indices = 0;
    buffer_offset = 0;

    frame_began = true;
}

void gfx_end_frame()
{
    assert((initialized == true));
    assert((frame_began == true));

    int location = glGetUniformLocation(shader_program, "projection");
    assert((location != -1));
    glUniformMatrix4fv(location, 1, GL_FALSE, projection_matrix[0]);

    glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0);

    frame_began = false;
}

void gfx_render_tile(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, vec2 pos, bool h_flip)
{
    assert((initialized == true));
    assert((frame_began == true));
    assert((num_indices + 6 < RENDERER_INDEX_BUFFER_COUNT));

    float width_in_game = tileset->in_game_width;
    float height_in_game = tileset->in_game_height;

    // Not using this data
    vertex vertices[4] = {0};
    glm_vec2_copy((vec2){-width_in_game/3,  height_in_game/3}, vertices[0].pos);
    glm_vec2_copy((vec2){0.0f, 0.0f},                          vertices[0].uv);
    glm_vec2_copy((vec2){ width_in_game/3,  height_in_game/3}, vertices[1].pos);
    glm_vec2_copy((vec2){1.0f, 0.0f},                          vertices[1].uv);
    glm_vec2_copy((vec2){ width_in_game/3, -height_in_game/3}, vertices[2].pos);
    glm_vec2_copy((vec2){1.0f, 1.0f},                          vertices[2].uv);
    glm_vec2_copy((vec2){-width_in_game/3, -height_in_game/3}, vertices[3].pos);
    glm_vec2_copy((vec2){0.0f, 1.0f},                          vertices[3].uv);

    // Move the quad into world space
    glm_vec2_add(vertices[0].pos, pos, vertices[0].pos);
    glm_vec2_add(vertices[1].pos, pos, vertices[1].pos);
    glm_vec2_add(vertices[2].pos, pos, vertices[2].pos);
    glm_vec2_add(vertices[3].pos, pos, vertices[3].pos);

    // Move the quad in view space
    glm_vec2_sub(vertices[0].pos, camera_pos, vertices[0].pos);
    glm_vec2_sub(vertices[1].pos, camera_pos, vertices[1].pos);
    glm_vec2_sub(vertices[2].pos, camera_pos, vertices[2].pos);
    glm_vec2_sub(vertices[3].pos, camera_pos, vertices[3].pos);

    glNamedBufferSubData(vertex_buffer, buffer_offset, sizeof(vertices), vertices);

    buffer_offset += 4 * sizeof(vertex);
    num_indices += 6;
}

void gfx_render_tile_cam_fraction(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, vec2 pos,
                                  bool h_flip, float fraction)
{
    assert((initialized == true));
    assert((frame_began == true));
    assert((num_indices + 6 < RENDERER_INDEX_BUFFER_COUNT));

    float width_in_game = tileset->in_game_width;
    float height_in_game = tileset->in_game_height;

    vertex vertices[4] = {0};
    glm_vec2_copy((vec2){-width_in_game/2,  height_in_game/2}, vertices[0].pos);
    glm_vec2_copy((vec2){0.0f, 0.0f},                          vertices[0].uv);
    glm_vec2_copy((vec2){ width_in_game/2,  height_in_game/2}, vertices[1].pos);
    glm_vec2_copy((vec2){1.0f, 0.0f},                          vertices[1].uv);
    glm_vec2_copy((vec2){ width_in_game/2, -height_in_game/2}, vertices[2].pos);
    glm_vec2_copy((vec2){1.0f, 1.0f},                          vertices[2].uv);
    glm_vec2_copy((vec2){-width_in_game/2, -height_in_game/2}, vertices[3].pos);
    glm_vec2_copy((vec2){0.0f, 1.0f},                          vertices[3].uv);

    // Move the quad into world space
    glm_vec2_add(vertices[0].pos, pos, vertices[0].pos);
    glm_vec2_add(vertices[1].pos, pos, vertices[1].pos);
    glm_vec2_add(vertices[2].pos, pos, vertices[2].pos);
    glm_vec2_add(vertices[3].pos, pos, vertices[3].pos);

    // Move the quad in view space
    vec2 cam_fraction = {camera_pos[0] * fraction, camera_pos[1] * fraction};
    glm_vec2_sub(vertices[0].pos, cam_fraction, vertices[0].pos);
    glm_vec2_sub(vertices[1].pos, cam_fraction, vertices[1].pos);
    glm_vec2_sub(vertices[2].pos, cam_fraction, vertices[2].pos);
    glm_vec2_sub(vertices[3].pos, cam_fraction, vertices[3].pos);

    glNamedBufferSubData(vertex_buffer, buffer_offset, sizeof(vertices), vertices);

    buffer_offset += 4 * sizeof(vertex);
    num_indices += 6;
}