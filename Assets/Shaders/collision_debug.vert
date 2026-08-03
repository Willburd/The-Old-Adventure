#version 330

in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;
uniform vec3 uWorldPos;

out vec4 fragColor;

void main()
{
    fragColor = vec4(vertexNormal.x,vertexNormal.y,vertexNormal.z,1.0);
    fragColor *= 0.75;
    fragColor += vec4(0.25,0.25,0.25,0.25);
    gl_Position = mvp*vec4(vertexPosition, 1.0);
}