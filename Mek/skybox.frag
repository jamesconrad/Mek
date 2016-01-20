#version 330

in vec3 uvw;
out vec4 colour;

uniform samplerCube skybox;

void main()
{
	colour = texture(skybox, uvw);
	//colour  = vec4(uvw,1);
}