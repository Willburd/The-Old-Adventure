#version 330 core
in vec2 TexCoords;
in vec3 Normal;

uniform vec4 uColorSet;

out vec4 FragColor;

void main()
{
    FragColor = uColorSet;
    FragColor.x *= 0.75 + (Normal.x * 0.25);
    FragColor.y *= 0.75 + (Normal.y * 0.25);
    FragColor.z *= 0.75 + (Normal.z * 0.25);
}