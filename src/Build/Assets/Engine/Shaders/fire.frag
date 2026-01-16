#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in float GameTick;

uniform sampler2D uTexture0;
uniform vec4 uFireColor;

out vec4 FragColor;

void main()
{
    float center_dist = pow(clamp(distance(vec2(0.5,0.5), vec2(TexCoords.x,TexCoords.y)) * 2.3, 0.0, 1.0), 3.0);
    
    vec4 fire_blend = texture(uTexture0, TexCoords + vec2(0.0, -GameTick * 0.02));
    fire_blend.r *= clamp(1.0 - TexCoords.y, 0.0, 1.0) * (1.0 - fire_blend.r); // fade toward top
    FragColor = vec4(uFireColor.rgb, fire_blend.r * (1.0 - center_dist));
}