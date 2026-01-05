#version 330 core
in vec2 TexCoords;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    if(FragColor.a < 1) discard; // We don't care about transparency here
}