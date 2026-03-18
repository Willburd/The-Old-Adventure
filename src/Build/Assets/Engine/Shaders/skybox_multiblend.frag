#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in float GameTick;
flat in int UniqueID;

uniform sampler2D uTextureDay;
uniform sampler2D uTextureDusk;
uniform sampler2D uTextureNight;
uniform sampler2D uTextureDawn;
uniform sampler2D uTextureDayRain;
uniform sampler2D uTextureNightRain;

uniform float uDuskPerc;
uniform float uNightPerc;
uniform float uDawnPerc;
uniform float uRainPerc;

out vec4 FragColor;

void main()
{
    FragColor = mix( texture(uTextureDay, TexCoords), texture(uTextureDayRain, TexCoords), uRainPerc);
    FragColor = mix(FragColor, mix( texture(uTextureDusk, TexCoords), texture(uTextureDayRain, TexCoords), uRainPerc), uDuskPerc);
    FragColor = mix(FragColor, mix( texture(uTextureNight, TexCoords), texture(uTextureNightRain, TexCoords), uRainPerc), uNightPerc);
    FragColor = mix(FragColor, mix( texture(uTextureDawn, TexCoords), texture(uTextureDayRain, TexCoords), uRainPerc), uDawnPerc);
    FragColor.a = 1.0f;
}