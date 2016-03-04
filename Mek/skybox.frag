#version 430

in vec3 uvw;
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 Depth;
layout(location = 2) out vec4 Normal;

uniform samplerCube skybox;

void main()
{
	colour = texture(skybox, uvw);
	Depth = vec4(0, 0, 0, 1.0);
	Normal = vec4(0, 0, 0, 1.0);
	//colour  = vec4(uvw,1);
}