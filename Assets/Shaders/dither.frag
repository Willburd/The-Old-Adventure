#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

uniform sampler2D texture0;

out vec4 finalColor;

float random(vec2 c) {
    return fract(sin(dot(c.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    finalColor = texture(texture0, fragTexCoord);
    finalColor.rgb *= vec3(0.99 + (0.01 * random(fragTexCoord)));
}
