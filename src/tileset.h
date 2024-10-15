#ifndef tileset_H
#define tileset_H

#include <stdint.h>

#include <cglm/cglm.h>

typedef struct 
{
    uint32_t tileset_texture;
    uint32_t shader_program;
    uint8_t x_tile_count;
    uint8_t y_tile_count;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
    float in_game_width;
    float in_game_height;
} tileset;

tileset tileset_create(const char *path, uint8_t x_tile_count, uint8_t y_tile_count);
void tileset_delete(tileset *tileset);

void tileset_render(tileset *tileset, uint8_t tile_x, uint8_t tile_y, mat4 mvp, bool h_flip);

#endif // tileset_H