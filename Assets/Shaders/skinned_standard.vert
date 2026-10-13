#version 330

#define MAX_LIGHTS 48
in vec3 vertexPosition;
in vec2 vertexTexCoord;
in vec4 vertexColor;
in vec3 vertexNormal;
in vec4 vertexBoneIndices;
in vec4 vertexBoneWeights;

#define MAX_BONE_NUM 128
uniform mat4 mvp;
uniform mat4 matNormal;
uniform mat4 boneMatrices[MAX_BONE_NUM];
uniform vec3 uWorldPos;

uniform vec3 uFogColor;
uniform float uFogDistance;
uniform float uFogPower;

uniform int uLightCount;
uniform vec4 uLightPositions[MAX_LIGHTS]; // pos + radius
uniform vec4 uLightColors[MAX_LIGHTS]; // color + influence

out vec2 fragTexCoord;
out vec4 fragColor;
out vec3 fragNormal;
out vec4 fragLight;

vec3 solve_lights(vec3 pos)
{
    // Collect static and dynamic lights
    int static_light_count = 0;
    int dynamic_light_count = 0;
    float total_static_influence = 0.0;
    float darkness_void_influence = 0.0;
    vec3 static_light_blend = vec3(0.0, 0.0, 0.0);
    vec3 dynamic_light_blend = vec3(0.0, 0.0, 0.0);
    for(int j = 0; j < uLightCount; j++)
    {
        vec4 light_pos = uLightPositions[j];
        vec4 light_col = uLightColors[j];
        if(light_col.xyz == vec3(0.0,0.0,0.0)) // Special cave darkness handling
        {
            float influence = 1.0 - clamp(distance(light_pos.xyz, pos) / light_pos.w, 0.0, 1.0);
            if(influence > darkness_void_influence)
                darkness_void_influence = influence;
        }
        else if(light_pos.w < 9999.0) // Lights larger than 10000 are world lights
        {
            light_col.w *= 1.0 - clamp(distance(light_pos.xyz, pos) / light_pos.w, 0.0, 1.0);
            dynamic_light_blend += (light_col.xyz * light_col.w);
            dynamic_light_count += 1;
        }
        else
        {
            static_light_blend += light_col.xyz;
            total_static_influence += light_col.w;
            static_light_count += 1;
        }
    }
    // Static lights are summed together, and the remaining difference in alpha after averaging is dispersed to the dynamic lights
    dynamic_light_blend = vec3(dynamic_light_blend.x / dynamic_light_count, dynamic_light_blend.y / dynamic_light_count, dynamic_light_blend.z / dynamic_light_count);
    static_light_blend = vec3(static_light_blend.x / static_light_count, static_light_blend.y / static_light_count, static_light_blend.z / static_light_count);
    float static_light_influence = total_static_influence / static_light_count;
    float dynamic_light_influence = 1.0 - static_light_influence;

    return clamp(mix((static_light_blend * static_light_influence) + (dynamic_light_blend * dynamic_light_influence), vec3(0.0,0.0,0.0), darkness_void_influence), 0.0, 1.0);
}

void main()
{
    // Bone skinning
    int boneIndex0 = int(vertexBoneIndices.x);
    int boneIndex1 = int(vertexBoneIndices.y);
    int boneIndex2 = int(vertexBoneIndices.z);
    int boneIndex3 = int(vertexBoneIndices.w);

    vec4 skinnedPosition =
        vertexBoneWeights.x*(boneMatrices[boneIndex0]*vec4(vertexPosition, 1.0)) +
        vertexBoneWeights.y*(boneMatrices[boneIndex1]*vec4(vertexPosition, 1.0)) +
        vertexBoneWeights.z*(boneMatrices[boneIndex2]*vec4(vertexPosition, 1.0)) +
        vertexBoneWeights.w*(boneMatrices[boneIndex3]*vec4(vertexPosition, 1.0));

    vec4 skinnedNormal =
        vertexBoneWeights.x*(boneMatrices[boneIndex0]*vec4(vertexNormal, 0.0)) +
        vertexBoneWeights.y*(boneMatrices[boneIndex1]*vec4(vertexNormal, 0.0)) +
        vertexBoneWeights.z*(boneMatrices[boneIndex2]*vec4(vertexNormal, 0.0)) +
        vertexBoneWeights.w*(boneMatrices[boneIndex3]*vec4(vertexNormal, 0.0));
    skinnedNormal.w = 0.0;

    // Position calc
    gl_Position = mvp*skinnedPosition;
    fragNormal = normalize(vec3(matNormal*skinnedNormal));
    fragTexCoord = vertexTexCoord;

    // Fog calc
    float dist = distance(vec4(0.0), gl_Position);
    float dist_perc = clamp(dist / uFogDistance, 0.0, 1.0);
    fragColor = mix(vertexColor, vec4(uFogColor.xyz,0.0), pow(dist_perc, uFogPower));

    // Light calc
    fragLight = vec4( solve_lights(uWorldPos + skinnedPosition), 1.0 - pow(dist_perc, 100));
}