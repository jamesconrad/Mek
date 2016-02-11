#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 gWVP;//aligns to camera
out vec3 n;
out vec3 p;
out float h;

uniform float _min;
uniform float _max;

void main()
{
	vec4 glp = vec4(Position, 1);
	glp.y = glp.y / 512;
	//glp.x = glp.x / 512;
	//glp.z = glp.z / 512;
	//gl_Position = gWVP * vec4(Position , 1);
	gl_Position = gWVP * glp;

	p = Position;
	h = (Position.y - _min) / (_max - _min);
	n = Normal;
}