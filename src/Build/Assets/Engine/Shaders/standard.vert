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
    vec4 total_light_blend = vec4(0.0,0.0,0.0,0.0);
    for(int j = 0; j < uLightCount; j++)
    {
        vec4 pos = uLightPositions[j];
        vec4 col = uLightColors[j];
        float radius = pos.a;
        col.a = clamp(distance(pos, vec4(vPosition.x,vPosition.y,vPosition.z,0.0)) / radius, 0.0, 1.0);

        total_light_blend += col;
    }
    return total_light_blend / uLightCount;
}

void main()
{
    gl_Position = uProjection * uView * uTransform * vec4(vPosition, 1.0);
    TexCoords = vUv;
    Normal = vNormal;
    Color = vColor;

    Light = vec4(0.5, 0.5, 0.5, 1.0); // solve_lights();
}