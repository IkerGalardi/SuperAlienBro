#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdint.h>

#include <cglm/cglm.h>

typedef struct 
{
    uint32_t tilemap_texture;
    uint32_t shader_program;
    uint8_t x_tile_count;
    uint8_t y_tile_count;
    uint32_t vertex_array;
    uint32_t vertex_buffer;
} tilemap;

tilemap tilemap_create(const char *path, uint8_t x_tile_count, uint8_t y_tile_count);
void tilemap_delete(tilemap *tilemap);

void tilemap_render(tilemap *tilemap, uint8_t tile_x, uint8_t tile_y, mat4 mvp);

#endif // TILEMAP_H