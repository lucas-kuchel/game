#version 460 core

layout(location = 0) in vec3 vaPosition;
layout(location = 1) in vec4 vaColour;

layout(location = 0) out vec4 fragColour;

layout(std140, binding = 0) uniform Camera
{
    mat4 cProjection;
    mat4 cView;
};

void main()
{
    gl_Position = cProjection * cView * vec4(vaPosition, 1.0);

    fragColour = vaColour;
}