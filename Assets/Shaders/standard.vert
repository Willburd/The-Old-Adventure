#version 330

#define MAX_LIGHTS 24
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;
uniform vec3 uWorldPos;

uniform vec3 uFogColor;
uniform float uFogDistance;
uniform float uFogPower;

uniform int uLightCount;
uniform vec4 uLightPositions[MAX_LIGHTS]; // pos + radius
uniform vec4 uLightColors[MAX_LIGHTS]; // color + influence

out vec2 fragTexCoord;
out vec4 fragColor;
out vec4 fragLight;

vec3 solve_lights(vec3 pos)
{
    vec3 total_light_blend = vec3(0.0, 0.0, 0.0);
    float remaining = 1.0;

    for(int j = 0; j < uLightCount; j++)
    {
        vec4 light_pos = uLightPositions[j];
        vec4 light_col = uLightColors[j];

        // Get the amount of influence our light has
        float rad_influence = light_col.a;
        if(light_pos.w < 9999.0) // Lights larger than 10000 are world lights
        {
            rad_influence = 1.0 - clamp(distance(light_pos.xyz, pos) / light_pos.w, 0.0, 1.0) * light_col.a;
            if(rad_influence < 0.05)
                continue;
        }

        // If we are worth considering, put us into the mix with the other lights
        float affect_remain = remaining;
        vec3 light_affect = light_col.rgb;
        if(light_affect == vec3(0.0,0.0,0.0)) // Special cave darkness handling
        {
            light_affect = vec3(-10.0,-10.0,-10.0);
            affect_remain = 1.0; // Force full intensity regardless of other lights
        }
        total_light_blend += light_affect * rad_influence * affect_remain;
        remaining *= (1.0 - rad_influence);
    }
    return clamp(total_light_blend, 0.0, 1.0);
}

void main()
{
    gl_Position = mvp*vec4(vertexPosition, 1.0);
    fragTexCoord = vertexTexCoord;
    // Fog calc
    float dist = distance(vec4(0.0), gl_Position);
    float dist_perc = clamp(dist / uFogDistance, 0.0, 1.0);
    fragColor = mix(vertexColor, vec4(uFogColor.xyz,0.0), pow(dist_perc, uFogPower));
    // Light calc
    fragLight = vec4( solve_lights(uWorldPos + vertexPosition), 1.0 - pow(dist_perc, 100));
}