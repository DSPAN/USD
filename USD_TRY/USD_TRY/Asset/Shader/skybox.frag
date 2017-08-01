#version 400
layout (location = 2) out vec4 gAlbedoSpec;
in vec3 texcoords;
uniform samplerCube cube_texture;
void main () 
{
  gAlbedoSpec= texture (cube_texture, texcoords);
  gAlbedoSpec.a=1;
}