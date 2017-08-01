#version 330 core


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D effect_map;

uniform mat4 projection;
uniform vec3 cameraPos;

in vec2 TexCoords;
out vec4 FragColor;

const float rayStep = 0.01;
const float minRayStep = 0.1;
const float maxSteps = 20;
const int numBinarySearchSteps = 5;
 //二分搜索 
vec2 BinarySearch(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    float depth;
    for(int i = 0; i < 5; i++)
    {
    	//NDC
        vec4 projectedCoord = projection * vec4(hitCoord, 1.0);
        projectedCoord /= projectedCoord.w;
        projectedCoord = projectedCoord * 0.5 + 0.5;
 
 
        depth = texture2D(gPosition, projectedCoord.xy).z;
        dDepth = hitCoord.z - depth;
        
        if(dDepth > 0.0)
            hitCoord += dir;
 
        dir *= 0.5;
        hitCoord -= dir;    
    }
    vec4 projectedCoord = projection * vec4(hitCoord, 1.0); 
    projectedCoord /= projectedCoord.w;
    projectedCoord = projectedCoord * 0.5 + 0.5;
 
    return projectedCoord.xy;
}

vec2 RayCast(vec3 dir, inout vec3 hitCoord, out float dDepth)
{
    dir *= 0.25f;

    for(int i = 0; i < 20; ++i) 
	{
        hitCoord               += dir; 

        vec4 projectedCoord     = projection * vec4(hitCoord, 1.0);
        projectedCoord      /= projectedCoord.w;
        projectedCoord       = projectedCoord * 0.5 + 0.5; 

        float depth             = texture(gPosition, projectedCoord.xy).z;
        dDepth                  = hitCoord.z - depth; 

        if(dDepth < 0.0)
        	return BinarySearch(dir,hitCoord,dDepth);
            //return projectedCoord.xy;
    }

    return vec2(0.0f);
}
void main()
{             
	
    float specular = texture(gAlbedoSpec, TexCoords).a;
 
    if(specular != 0.8)
    {
		vec3 Albedo = texture(effect_map, TexCoords).rgb;
    	FragColor =  vec4(Albedo,1);
        return;
    }
    
    vec3 viewNormal = texture2D(gNormal, TexCoords).xyz;
	vec3 viewPos= texture2D(gPosition, TexCoords).xyz;

    vec3 reflected = normalize(reflect(normalize(viewPos-cameraPos), normalize(viewNormal)));//反射方向 
    vec3 hitPos = viewPos;
    
    float dDepth;
    vec2 coords = RayCast(reflected * max(minRayStep, -viewPos.z), hitPos, dDepth);
    
    FragColor =  texture2D(effect_map, coords)*0.4+texture2D(effect_map, TexCoords)*0.6;

}  