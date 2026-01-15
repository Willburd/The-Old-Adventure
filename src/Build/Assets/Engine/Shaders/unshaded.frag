#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec4 Color;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    if(FragColor.a < 1.0) discard; // Alpha clip on texture
    FragColor = vec4(mix(FragColor.rgb, Color.rgb, Color.a), FragColor.a);
}