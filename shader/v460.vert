#version 460

layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 1 ) in vec3 VertexColor;
layout ( location = 3 ) in vec3 VertexNormal;
layout ( location = 4 ) in vec2 VertexTexture;

out vec3 Color;
out vec3 Normal;
out vec2 Texture;

uniform mat4 MVP;

void main()
{
    Color = VertexColor;
    Normal = VertexNormal;
    Texture = VertexTexture;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}