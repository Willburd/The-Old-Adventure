#version 330 core
in float GameTick;

uniform vec4 uColorSet;

out vec4 FragColor;

void main()
{
    FragColor = uColorSet;
}