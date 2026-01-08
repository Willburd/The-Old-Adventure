#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec4 Color;
in vec4 Light;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    FragColor *= vec4(Color.r * Color.a,Color.g * Color.a,Color.b * Color.a, 1.0);
    FragColor *= vec4(Light.r * Light.a, Light.g * Light.a, Light.b * Light.a, 1.0);
}