#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 FireColor;
uniform int Identity;
uniform float Animator;

out vec4 finalColor;

void main()
{
    float center_dist = pow(clamp(distance(vec2(0.5,0.5), vec2(fragTexCoord.x,fragTexCoord.y)) * 2.1, 0.0, 1.0), 2.0);

    vec4 fire_blend = texture(texture0, ((fragTexCoord * 1.8) + vec2( Identity * 0.13, Animator * 1.5f )));
    float burn_radius = fire_blend.r * (1.0 - center_dist);

    finalColor = vec4(FireColor.rgb, burn_radius);
    finalColor.a -= (1.0 - fragTexCoord.y) * 1.1; // fade toward top
    finalColor.a += fire_blend.r * burn_radius * 0.5;
    finalColor *= 0.5;

    vec4 fire_blend_alt = texture(texture0, (fragTexCoord + vec2( Identity * 0.21, Animator * 0.84f )) * 0.95);
    finalColor = pow(finalColor, fire_blend_alt);

    finalColor = clamp(finalColor, 0.0, 1.0);
}
