#version 420 core

in Geometry
{
    vec2 v_uv;
};

out vec4 o_fragment;


layout(binding = 0) uniform sampler2D u_character_tileset;
uniform vec2 u_tileset_size;
uniform vec2 u_tileset_render;

void main()
{
    vec2 tileset_uv = v_uv / u_tileset_size + u_tileset_render / u_tileset_size;
    o_fragment = texture(u_character_tileset, tileset_uv);
} 