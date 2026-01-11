#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec4 Color;
in vec4 Light;

uniform sampler2D uTextureDay;
uniform sampler2D uTextureDusk;
uniform sampler2D uTextureNight;
uniform sampler2D uTextureDawn;

uniform float uDuskPerc;
uniform float uNightPerc;
uniform float uDawnPerc;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTextureDay, TexCoords);
    FragColor = mix(FragColor, texture(uTextureDusk, TexCoords), uDuskPerc);
    FragColor = mix(FragColor, texture(uTextureNight, TexCoords), uNightPerc);
    FragColor = mix(FragColor, texture(uTextureDawn, TexCoords), uDawnPerc);
}