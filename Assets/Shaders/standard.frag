#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec4 fragLight;

uniform sampler2D texture0;
uniform vec4 uBlendColor;

out vec4 finalColor;

void main()
{
    // Get texture
    finalColor = texture(texture0, fragTexCoord);
    if(finalColor.a <= 0.5) discard; // Alpha clip on texture
    // Blend fog
    finalColor = vec4(fragColor.r * finalColor.r * uBlendColor.r, fragColor.g * finalColor.g * uBlendColor.g, fragColor.b * finalColor.b * uBlendColor.b, finalColor.a);
    if(finalColor.a <= 0.5) discard; // Alpha clip on texture
    finalColor.a = 1.0; // We made it, so we must be drawn
    // Blend lights
    if(fragLight.r > 1.0 || fragLight.g > 1.0 || fragLight.b > 1.0)
        finalColor = vec4(1.0, 1.0, 1.0, finalColor.a); // Cave exit light
    else
        finalColor *= fragLight; // standard light
}
