#version 330

layout (location = 0) in vec3 position;
out vec3 uvw;

uniform mat4 m;
vec4 p;

void main()
{
	p = vec4(position * 16, 1.0);
	gl_Position = m * p;
	uvw = position;
}