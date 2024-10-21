#ifndef GFX_H
#define GFX_H

#include <stdint.h>
#include <stddef.h>
#include <cglm/cglm.h>

typedef struct 
{
    uint32_t tileset_texture;
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

    size_t *rendered_variations;
} gfx_background;

/**
 * @brief Initializes renderer internal structures and sets the projection matrix 
 *
 * @param proj_matrix projection matrix used by the camera.
 */
void gfx_initialize(mat4 proj_matrix);

/**
 * @brief Function to be called when the frame starts, prepares the renderer for drawing. 
 *
 * @param camera_position position of the camera in the world
 */
void gfx_begin_frame(vec2 camera_position);

/**
 * @brief Function to be called when the frame ends 
 */
void gfx_end_frame();

/**
 * @brief Creates all the necessary resources to render tiles from a tileset file. 
 * 
 * Its important to `gfx_delete_tileset` when the tileset is no longer in use in order to save up
 * memory.
 * 
 * @param path path to the file containing the tileset image
 * @param x_tile_count number of tiles in the X axis
 * @param y_tile_count number of tiles in the Y axis
 * @return The tileset object
 */
gfx_tileset gfx_create_tileset(const char *path, uint8_t x_tile_count, uint8_t y_tile_count);

/**
 * @brief Deletes all the necessary resources used to render tiles and frees up memory.
 * 
 * @param tileset tileset to be deleted
 */
void gfx_delete_tileset(gfx_tileset *tileset);

/**
 * @brief Renders a tile in the tileset.
 * 
 * @param tileset tileset to be rendered
 * @param tile_x position of the tile in the X axis
 * @param tile_y position of the tile in the Y axis
 * @param mvp model view projection matrix used to transform the object
 * @param h_flip true in order to flip in the x axis, false otherwise
 */
void gfx_render_tile(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, vec2 pos, bool h_flip);

/**
 * @brief Renders a tile in the tileset taking into account a fraction of the camera position.
 * 
 * @param tileset tileset to be rendered
 * @param tile_x position of the tile in the X axis
 * @param tile_y position of the tile in the Y axis
 * @param mvp model view projection matrix used to transform the object
 * @param h_flip true in order to flip in the x axis, false otherwise
 * @param fraction fraction of the camera position to take into account
 */
void gfx_render_tile_cam_fraction(gfx_tileset *tileset, uint8_t tile_x, uint8_t tile_y, vec2 pos,
                                  bool h_flip, float fraction);

/**
 * @brief Renders the correct animation frame
 *
 * @param animation animation to be rendered
 * @param mvp model view projection matrix used to transform the object
 * @param h_flip true in order to flip the x axis, false otherwise
 */
void gfx_render_animation(gfx_animation *animation, vec2 pos, bool h_flip);

/**
 * @brief Creates all the necessary resources to render the background 
 * 
 * @param tileset tileset to be used to render the background
 * @param variations variations of the same background to be added randomly
 * @param variation_count number of variations passed in `variations`
 * @return background object to be used on `gfx_render_background`
 */
gfx_background gfx_create_background(gfx_tileset *tileset, size_t *variations, size_t variation_count);

/**
 * @brief Renders the background
 * 
 * @param background background to be rendered
 */
void gfx_render_background(gfx_background *background);


#endif // GFX_H