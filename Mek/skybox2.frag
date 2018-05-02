#version 430

in vec3 uvw;
layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 Depth;
layout(location = 2) out vec4 Normal;
layout(location = 3) out vec4 LightObscurers;

uniform samplerCube skybox;

void main()
{
	colour = texture(skybox, uvw);

	Depth = colour;
	Normal = colour;
	//LightObscurers = colour;
	LightObscurers = vec4(vec3(0.1), 1.0);
	//These two are the correct ones
	//Depth = vec4(0, 0, 0, 1.0);
	//Normal = vec4(0, 0, 0, 1.0);


	//colour  = vec4(uvw,1);
}