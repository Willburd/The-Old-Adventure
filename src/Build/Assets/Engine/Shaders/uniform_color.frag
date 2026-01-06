#version 330 core

uniform vec4 uColorSet;

out vec4 FragColor;

void main()
{
    FragColor = uColorSet;
}