#version 330 core

#define PI 3.1415926
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gMateria;
uniform vec3 camPos;



vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

void main()
{             

    float s = texture(gAlbedoSpec, TexCoords).a;
	float metallic=texture(gMateria, TexCoords).r;
	float roughness=texture(gMateria, TexCoords).g;
	
	vec3 lightPositions=vec3(0,1,-3);
	vec3 lightColors=vec3(0.9,0.9,0.9);
	vec3 albedo=texture(gAlbedoSpec, TexCoords).rgb; 
	vec3 WorldPos=texture2D(gPosition, TexCoords).rgb;
	vec3 Normal=texture2D(gNormal, TexCoords).rgb;
	vec3 N = normalize(Normal);
	vec3 V = normalize(camPos - WorldPos);
	vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
	
	vec3 Lo = vec3(0.0);
	
	vec3 L = normalize(lightPositions - WorldPos);
	vec3 H = normalize(V + L);
	float distance    = length(lightPositions - WorldPos);
	vec3 radiance     = lightColors*5;
	
	float NDF = DistributionGGX(N, H, roughness);        
	float G   = GeometrySmith(N, V, L, roughness);      
	vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);       
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;
	
	vec3 specular=(NDF * G * F)/(4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001);
	float NdotL = max(dot(N, L), 0.0);
	Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	
	vec3 ambient = 0.03* albedo;
                                           
	vec3 color =ambient+Lo;
	//color = color / (color + vec3(1.0));
    //color = pow(color, vec3(1.0/2.2));
    
    FragColor=vec4(color,1);

}  