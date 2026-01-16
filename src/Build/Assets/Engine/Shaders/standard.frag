#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 Color;
in vec4 Light;

uniform sampler2D uTexture0;

out vec4 FragColor;

void main()
{
    FragColor = texture(uTexture0, TexCoords);
    if(FragColor.a < 1.0) discard; // Alpha clip on texture
    if(Color.rgb == vec3(0.0)) 
    {
        // Don't bother with lights, these are either pits, caves, etc
        FragColor = vec4(0.0,0.0,0.0,1.0);
        return;
    }
    FragColor = vec4(FragColor.rgb * Color.rgb, FragColor.a);
    FragColor *= Light;
    if(FragColor.a < 0.001) discard;
}