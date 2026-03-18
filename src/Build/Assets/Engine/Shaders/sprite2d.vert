#version 330 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vUv;
layout (location = 2) in vec3 vNormal;
layout (location = 3) in vec3 vColor;

uniform mat4 uTransform;
uniform mat4 uView;
uniform mat4 uProjection;
uniform float uGameTick;
uniform int uUniqueID;

uniform vec2 uSpritePos;
uniform vec2 uSpriteSize;
uniform mat4 uDrawOffset;
uniform vec3 uDrawColor;

out vec2 TexCoords;
out vec3 Normal;
out vec3 Color;
out float GameTick;
flat out int UniqueID;

void main()
{
    gl_Position = uProjection * uView * uTransform * uDrawOffset * vec4(vPosition, 1.0);
    TexCoords = vec2(uSpritePos.x + mix(0.0, uSpriteSize.x, vUv.x), uSpritePos.y + mix(0.0, uSpriteSize.y, vUv.y));
    Normal = vNormal;
    Color = uDrawColor; // Instead of vertex color
    GameTick = uGameTick;
    UniqueID = uUniqueID;
}