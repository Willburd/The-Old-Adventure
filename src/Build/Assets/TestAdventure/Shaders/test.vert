#version 330 core
#define MAX_LIGHTS 16
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec4 vColor;

// Position
uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;
// Lights
uniform int uLightCount;
uniform vec4 uLightPositions[MAX_LIGHTS]; // pos + radius
uniform vec4 uLightColors[MAX_LIGHTS];

// Output
out vec2 TexCoords;
out vec3 Normal;
out vec4 Color;
out vec4 Light;

void main()
{
    gl_Position = uTransform * vec4(vPos, 1.0);
    TexCoords = vUv;
}