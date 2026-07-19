#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec4 fragLight;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

void main()
{
    // Get texture
    finalColor = texture(texture0, fragTexCoord);
    if(finalColor.a < 1.0) discard; // Alpha clip on texture
    // Blend fog
    finalColor = vec4(finalColor.rgb * fragColor.rgb, fragColor.a);
    if(finalColor.a < 0.001) discard;
    // Blend lights
    if(fragLight.r > 1.0 || fragLight.g > 1.0 || fragLight.b > 1.0)
        finalColor = vec4(1.0, 1.0, 1.0, finalColor.a); // Cave exit light
    else
        finalColor *= fragLight; // standard light
}
