#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec4 vColor;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 TexCoords;
out vec3 Normal;

void main()
{
    gl_Position = uProjection * uView * uTransform * vec4(vPosition, 1.0);
    TexCoords = vUv;
    Normal = vNormal;
}