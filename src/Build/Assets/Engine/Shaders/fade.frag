#version 330 core
in vec4 Color;

uniform float uFade;

out vec4 FragColor;

void main()
{
    FragColor = vec4(Color.rgb, uFade);
    if(FragColor.a < 0.01) discard;
}