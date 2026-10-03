#version 330
#extension GL_ARB_explicit_uniform_location : enable

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

uniform float day_intensity;
uniform float dawn_intensity;
uniform float dusk_intensity;
uniform float rain_intensity;

uniform sampler2D texture0; // sky
uniform sampler2D texture1; // clouds

uniform vec4 sky_color;
uniform vec4 cloud_color;

out vec4 finalColor;

void main()
{
    float sky = texture(texture0, fragTexCoord).x;
    float clds = texture(texture1, fragTexCoord).x;
    finalColor = sky_color * vec4(sky,sky,sky,1.0);
    finalColor = mix(finalColor, cloud_color, clds);
}
