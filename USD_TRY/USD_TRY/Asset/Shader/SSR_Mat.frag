#version 330 core
layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 gMateria;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D DiffuseMap;
uniform float metallic;
uniform float roughness;
uniform float ao;

const float NEAR = 1.0; // 投影矩阵的近平面
const float FAR = 100.0f; // 投影矩阵的远平面
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // 回到NDC
    return (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));    
}
void main()
{          
    gPosition.rgb = FragPos;
    gPosition.a = LinearizeDepth(gl_FragCoord.z);
    gNormal = normalize(Normal);
    gAlbedoSpec.rgb = texture(DiffuseMap, vec2(TexCoords.x,1-TexCoords.y)).rgb;
    gAlbedoSpec.a = 0.8;
    gMateria=vec3(metallic,roughness,ao); 
    
}
