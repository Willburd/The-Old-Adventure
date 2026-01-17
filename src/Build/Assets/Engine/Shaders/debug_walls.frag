#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in float GameTick;
flat in int UniqueID;

out vec4 FragColor;

float wall_threshold = 0.4; // Must match wall_y_threshold in WorldGeo.cs
float normal_blend = 0.6f;

void main()
{
    FragColor = vec4( Normal.x * normal_blend, 1.0, Normal.z * normal_blend,  1.0);
    if(Normal.y > wall_threshold)  FragColor = vec4( Normal.x * normal_blend, Normal.z * normal_blend, 1.0                    , 1.0);
    if(Normal.y < -wall_threshold) FragColor = vec4( 1.0                    , Normal.y * normal_blend, Normal.z * normal_blend, 1.0);
}