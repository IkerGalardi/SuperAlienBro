#version 420 core

in Geometry
{
    vec2 v_uv;
};

out vec4 o_fragment;

layout(binding = 0) uniform sampler2D u_character_tilemap;

void main()
{
    o_fragment = texture(u_character_tilemap, v_uv);
} 