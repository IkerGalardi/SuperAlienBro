#version 420 core

in Geometry
{
    vec2 v_uv;
};

out vec4 o_fragment;


layout(binding = 0) uniform sampler2D u_character_tilemap;
uniform vec2 u_tilemap_size;
uniform vec2 u_tilemap_render;

void main()
{
    vec2 tilemap_uv = (v_uv / u_tilemap_size) + u_tilemap_render;
    o_fragment = texture(u_character_tilemap, tilemap_uv);
} 