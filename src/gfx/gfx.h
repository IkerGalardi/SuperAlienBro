#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <stddef.h>
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

typedef struct
{
    uint8_t x, y;
} gfx_animation_frame;

typedef struct
{
    gfx_tileset *tileset;

    gfx_animation_frame frames[5];
    size_t frame_count;

    double seconds_per_frame;
} gfx_animation;


typedef struct
{
    gfx_tileset *tileset;

    size_t variations[5];
    size_t variation_count;

    size_t _present_variations[3];
} gfx_background;


gfx_tileset gfx_create_tileset(const char *path, uint8_t x_tile_count, uint8_t y_tile_count);

void gfx_delete_tileset(gfx_tileset *tileset);

void gfx_render_tile(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, mat4 mvp, bool h_flip);

void gfx_render_animation(gfx_animation *animation, mat4 mvp, bool h_flip);

gfx_background gfx_create_background(gfx_tileset *tileset, size_t *variations, size_t variation_count);

void gfx_render_background(gfx_background *background);


#endif // GFX_H