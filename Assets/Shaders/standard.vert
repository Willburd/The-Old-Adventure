#version 330

#define MAX_LIGHTS 16
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec3 vertexNormal;
in vec4 vertexColor;

uniform mat4 mvp;

uniform vec3 uFogColor;
uniform float uFogDistance;
uniform float uFogPower;

uniform vec4 uLightPositions[MAX_LIGHTS]; // pos + radius
uniform vec4 uLightColors[MAX_LIGHTS];
uniform int uLightCount;

out vec2 fragTexCoord;
out vec4 fragColor;
out vec4 vertLight;

vec3 solve_lights()
{
    vec3 total_light_blend = vec3(0.0);
    float remaining = 1.0;

    for(int j = 0; j < uLightCount; j++)
    {
        vec4 light_pos = uLightPositions[j];
        vec4 light_col = uLightColors[j];

        // Get the amount of influence our light has
        float rad_influence = 1.0;
        if(light_pos.w < 9999999.0) 
        {
            rad_influence = 1.0 - clamp(distance(light_pos.xyz, vertexPosition) / light_pos.w, 0.0, 1.0);
        }
        rad_influence *= light_col.a;
        if(rad_influence > 0.01)
        {
            // If we are worth considering, put us into the mix with the other lights
            total_light_blend += clamp(light_col.rgb, 0.0, 1.1) * rad_influence * remaining;
            remaining *= (1.0 - rad_influence);
        }
    }

    return total_light_blend;
}

void main()
{
    gl_Position = mvp*vec4(vertexPosition, 1.0);
    fragTexCoord = vertexTexCoord;

    float dist = distance(vec4(0.0), gl_Position);
    float dist_perc = clamp(dist / uFogDistance, 0.0, 1.0);
    vertLight = vec4(solve_lights(), 1.0 - pow(dist_perc, 100));
    fragColor = clamp(mix(vertexColor, vec4(uFogColor,0.0), pow(dist_perc, uFogPower)), 0.0, 1.0);
}