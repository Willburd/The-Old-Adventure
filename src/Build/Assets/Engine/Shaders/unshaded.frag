#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec4 Color;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    FragColor.rgb *= Color.rgb;
}