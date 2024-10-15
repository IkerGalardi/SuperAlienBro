#version 420 core

in Geometry
{
    vec2 v_uv;
};

out vec4 o_fragment;

layout(binding = 0) uniform sampler2D u_tileset;
uniform bool u_horizontal_flip;
uniform vec2 u_tileset_size;
uniform vec2 u_tileset_render;

void main()
{
    vec2 uv = v_uv;
    if (u_horizontal_flip) {
        uv = vec2(1 - v_uv.x, v_uv.y);
    }

    vec2 tileset_uv = uv / u_tileset_size + u_tileset_render / u_tileset_size;
    o_fragment = texture(u_tileset, tileset_uv);
} 