#version 420 core

layout (location = 0) in vec2 a_pos;
layout (location = 1) in vec2 a_uv;

uniform mat4 u_mvp;

out Geometry
{
    vec2 v_uv;
};

void main()
{
    gl_Position = u_mvp * vec4(a_pos.x, a_pos.y, -1.0, 1.0);
    v_uv = a_uv;
}