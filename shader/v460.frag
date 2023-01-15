#version 460

in vec3 Color;
in vec3 Normal;
in vec2 Texture;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(Normal * 0.5 + 0.5, 1.0);
}