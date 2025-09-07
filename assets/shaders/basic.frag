#version 460 core

layout(location = 0) in vec4 inColour;

layout(location = 0) out vec4 fragColour;

void main()
{
    fragColour = inColour;
}