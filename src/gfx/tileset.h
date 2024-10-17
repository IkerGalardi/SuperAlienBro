#ifndef TILESET_H
#define TILESET_H

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
} gfx_tileset;

gfx_tileset gfx_create_tileset(const char *path, uint8_t x_tile_count, uint8_t y_tile_count);
void gfx_delete_tileset(gfx_tileset *tileset);

void gfx_render_tile(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, mat4 mvp, bool h_flip);

#endif // TILESET_H