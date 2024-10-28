#include "gfx.h"

#include <glad/glad.h>

static bool initialized = false;
static bool frame_began = false;

static mat4 projection_matrix;
static vec2 camera_pos;

static uint32_t shader_program;

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

    uint32_t vertex_shader = create_shader(GL_VERTEX_SHADER, "res/shaders/tileset.vert.glsl");
    uint32_t fragment_shader = create_shader(GL_FRAGMENT_SHADER, "res/shaders/tileset.frag.glsl");
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glValidateProgram(shader_program);

    glm_mat4_copy(proj_matrix, projection_matrix);

    initialized = true;
}

void gfx_begin_frame(vec2 camera_position)
{
    assert((initialized == true));
    assert((frame_began == false));

    glClear(GL_COLOR_BUFFER_BIT);

    glm_vec2_copy(camera_position, camera_pos);

    frame_began = true;
}

void gfx_end_frame()
{
    assert((initialized == true));
    assert((frame_began == true));

    frame_began = false;
}

void gfx_render_tile(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, vec2 pos, bool h_flip)
{
    assert((initialized == true));
    assert((frame_began == true));

    glBindTextureUnit(0, tileset->tileset_texture);
    glBindVertexArray(tileset->vertex_array);
    glUseProgram(shader_program);

    int size_location = glGetUniformLocation(shader_program, "u_tileset_size");
    int render_location = glGetUniformLocation(shader_program, "u_tileset_render");
    assert((size_location != -1 && render_location != -1));
    glUniform2f(size_location, (float)tileset->x_tile_count, (float)tileset->y_tile_count);
    glUniform2f(render_location, (float)tile_x, (float)tile_y);

    mat4 mvp;
    calc_mvp(pos, mvp);

    int mvp_location = glGetUniformLocation(shader_program, "u_mvp");
    assert((mvp_location != -1));
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp[0]);

    int h_flip_location = glGetUniformLocation(shader_program, "u_horizontal_flip");
    assert((h_flip_location != -1));
    glUniform1i(h_flip_location, h_flip);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void gfx_render_tile_cam_fraction(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, vec2 pos,
                                  bool h_flip, float fraction)
{
    assert((initialized == true));
    assert((frame_began == true));

    glBindTextureUnit(0, tileset->tileset_texture);
    glBindVertexArray(tileset->vertex_array);
    glUseProgram(shader_program);

    int size_location = glGetUniformLocation(shader_program, "u_tileset_size");
    int render_location = glGetUniformLocation(shader_program, "u_tileset_render");
    assert((size_location != -1 && render_location != -1));
    glUniform2f(size_location, (float)tileset->x_tile_count, (float)tileset->y_tile_count);
    glUniform2f(render_location, (float)tile_x, (float)tile_y);

    mat4 mvp;
    calc_mvp_cam_fraction(pos, fraction, mvp);

    int mvp_location = glGetUniformLocation(shader_program, "u_mvp");
    assert((mvp_location != -1));
    glUniformMatrix4fv(mvp_location, 1, GL_FALSE, mvp[0]);

    int h_flip_location = glGetUniformLocation(shader_program, "u_horizontal_flip");
    assert((h_flip_location != -1));
    glUniform1i(h_flip_location, h_flip);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}