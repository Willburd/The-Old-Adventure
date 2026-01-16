#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in float GameTick;

out vec4 FragColor;

void main()
{
    FragColor = vec4( Normal.x, Normal.y, Normal.z, 1.0);
}