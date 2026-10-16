#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform sampler2D lut_tex;

out vec4 finalColor;

void main()
{
    // solve lookup
    vec4 screen_col = texture(texture0, fragTexCoord);
    float rx = screen_col.r * 0.123046875;
    float gy = screen_col.g * 0.123046875;

    float blue_index = floor( (screen_col.b * 255.0) / 4.0);
    float bx = mod( blue_index, 8.0)    / 8.0;
    float by = floor( blue_index / 8.0) / 8.0;

    // preserve alpha
    finalColor = vec4(texture(lut_tex, vec2( clamp(bx + rx, 0.0, 0.998), clamp(by + gy, 0.0, 0.998))).rgb, screen_col.a);
}
