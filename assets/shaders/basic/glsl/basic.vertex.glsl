#version 460

layout(location = 0) in vec3 in_var_POSITION;
layout(location = 1) in vec4 in_var_COLOR0;
layout(location = 0) out vec4 out_var_COLOR0;

void main()
{
    gl_Position = vec4(in_var_POSITION, 1.0);
    out_var_COLOR0 = in_var_COLOR0;
}

