#version 460

layout(binding = 0, std140) uniform type_Camera
{
    layout(row_major) mat4 Projection;
    layout(row_major) mat4 View;
} Camera;

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec4 in_var_COLOR0;
layout(location = 0) out vec4 out_var_COLOR0;

mat4 spvWorkaroundRowMajor(mat4 wrap) { return wrap; }

void main()
{
    gl_Position = spvWorkaroundRowMajor(Camera.Projection) * (spvWorkaroundRowMajor(Camera.View) * vec4(in_var_POSITION, 1.0));
    out_var_COLOR0 = in_var_COLOR0;
}

