#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec4 blend_color;

out vec4 finalColor;

void main()
{
    finalColor = texture(texture0, fragTexCoord) * blend_color;
}
