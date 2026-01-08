#version 330 core
in vec2 TexCoords;

uniform vec4 uColorSet;

out vec4 FragColor;

void main()
{
    FragColor = uColorSet;
    FragColor.x *= TexCoords.x * 0.9;
    FragColor.y *= TexCoords.y * 0.9;
    FragColor.z *= TexCoords.x * 0.9;
}