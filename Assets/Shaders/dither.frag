#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform int dither_width;
uniform int dither_height;

out vec4 finalColor;

float random(vec2 c) {
    return fract(sin(dot(c.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

void main()
{
    float flip = 0.0;
    float pix_x = fragTexCoord.x * float(dither_width);
    float pix_y = fragTexCoord.y * float(dither_height);

    finalColor = texture(texture0, fragTexCoord);
    if(mod(pix_y,2.0) < 1)
        flip = 1.0;
    if(mod(pix_x + flip,2.0) < 1)
        finalColor *= vec4(0.98, 0.98, 0.98, finalColor.a);
    finalColor *= vec4(vec3(0.98 + (random(fragTexCoord) * 0.02)).rgb, finalColor.a);
}
