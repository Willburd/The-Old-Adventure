#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec4 fragLight;
in vec3 fragWorldPos;

uniform sampler2D texture0;

out vec4 finalColor;

void main()
{
    // Get texture
    finalColor = texture(texture0, vec2(fragWorldPos.x * 0.15, fragWorldPos.z * 0.15));
    // Blend fog
    finalColor = vec4(fragColor.r * finalColor.r, fragColor.g * finalColor.g, fragColor.b * finalColor.b, finalColor.a);
    if(finalColor.a < 0.001) discard;
    // Blend lights
    finalColor *= fragLight;
}
