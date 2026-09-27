#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec4 fragLight;
in vec3 fragWorldPos;

uniform float uTime;
uniform sampler2D texture0;

out vec4 finalColor;

void main()
{
    // Get texture
    finalColor = texture(texture0, vec2(fragWorldPos.x * 0.05, fragWorldPos.z * 0.05) + vec2(uTime * 0.03,uTime * 0.03));
    vec4 alt_col = texture(texture0, vec2(fragWorldPos.x * 0.01, fragWorldPos.z * 0.01) + vec2(uTime * 0.02,uTime * 0.02));
    finalColor = mix(finalColor, alt_col, pow(alt_col.b, 7.0) * 0.75);
    // Blend fog
    finalColor = vec4(fragColor.r * finalColor.r, fragColor.g * finalColor.g, fragColor.b * finalColor.b, finalColor.a);
    if(finalColor.a < 0.001) discard;
    // Blend lights
    finalColor *= fragLight;
}
