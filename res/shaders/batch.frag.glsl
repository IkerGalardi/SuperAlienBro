#version 420 core

in Geometry
{
    vec2 v_uv;
};

out vec4 o_fragment;

layout(binding = 0) uniform sampler2D u_tileset;

void main()
{
    o_fragment = vec4(0.5, 0.5, 0.0, 1.0);
} 