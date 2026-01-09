#version 330 core
#define MAX_LIGHTS 16
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec4 vColor;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

uniform vec4 uLightPositions[MAX_LIGHTS]; // pos + radius
uniform vec4 uLightColors[MAX_LIGHTS];
uniform int uLightCount;

out vec2 TexCoords;
out vec3 Normal;
out vec4 Color;
out vec4 Light;

vec4 solve_lights()
{
    vec4 total_light_blend = vec4(0.0, 0.0, 0.0, 1.0);
    for(int j = 0; j < uLightCount; j++)
    {
        vec4 light_pos = uLightPositions[j];
        vec4 light_col = uLightColors[j];
        float light_intensity = light_col.a;
        float rad_influence = 1.0;
        if(light_pos.w < 9999999.0) rad_influence = 1.0 - clamp(distance(light_pos.xyz, vPosition) / light_pos.w, 0.0, 1.0);

        total_light_blend.r = max(total_light_blend.r, light_col.r * rad_influence * light_intensity);
        total_light_blend.g = max(total_light_blend.g, light_col.g * rad_influence * light_intensity);
        total_light_blend.b = max(total_light_blend.b, light_col.b * rad_influence * light_intensity);
    }
    return total_light_blend;
}

void main()
{
    gl_Position = uProjection * uView * uTransform * vec4(vPosition, 1.0);
    TexCoords = vUv;
    Normal = vNormal;
    Color = vColor;

    Light = solve_lights();
}