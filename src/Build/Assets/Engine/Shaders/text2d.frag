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
    FragColor.a = 1.0 - pow(1.0 - FragColor.r, 2.5); // alpha is white channel
    FragColor = vec4(Color.rgb, FragColor.a);
    FragColor.rgb * (0.66 + (FragColor.a * 0.33));
}