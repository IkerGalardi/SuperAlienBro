#version 420 core

in Geometry
{
    vec2 v_uv;
};

out vec4 o_fragment;

void main()
{
    o_fragment = vec4(v_uv.y, v_uv.x, 1.0, 1.0);
}