#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vUv;

uniform mat4 uTransform;

out vec2 TexCoords;

void main()
{
    gl_Position = uTransform * vec4(vPos, 1.0);
    TexCoords = vUv;
}