#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
    finalColor = colDiffuse*fragColor;
}
