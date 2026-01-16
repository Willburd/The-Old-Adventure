#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in float GameTick;

uniform sampler2D uTexture0;
uniform vec3 uFireColor;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    //FragColor *= uFireColor;

    float x_intensity = (abs(TexCoords.x - 0.5) * 2.0);
    float y_intensity = (abs(TexCoords.y - 0.5) * 2.0);

    FragColor.a = 1.0 - x_intensity;
    FragColor.a *= 1.0 - y_intensity;

}