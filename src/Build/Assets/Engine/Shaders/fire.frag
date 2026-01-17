#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in float GameTick;
flat in int UniqueID;

uniform sampler2D uTexture0;
uniform vec4 uFireColor;

out vec4 FragColor;

void main()
{
    float center_dist = pow(clamp(distance(vec2(0.5,0.5), vec2(TexCoords.x,TexCoords.y)) * 2.3, 0.0, 1.0), 7.0);
    
    vec4 fire_blend = texture(uTexture0, (TexCoords + vec2(UniqueID * 0.001, (-GameTick * 0.04) + (UniqueID * 0.001))) * vec2(2f, 1.5f));
    float burn_radius = fire_blend.r * (1.0 - center_dist);

    FragColor = vec4(uFireColor.rgb, burn_radius);
    FragColor.a -= TexCoords.y * 2.5; // fade toward top
    FragColor.a += fire_blend.r * burn_radius * 0.5;
    FragColor = clamp(FragColor, 0.0, 1.0);
}