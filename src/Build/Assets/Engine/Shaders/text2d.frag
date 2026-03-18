#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec4 Color;
in float GameTick;
flat in int UniqueID;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    if(FragColor.r < 0.1) discard;
    float edge = 1.0 - pow(1.0 - FragColor.r, 2.5); // alpha is white channel
    FragColor = vec4(mix(vec3(0.3), Color.rgb, edge), 1.0);
}