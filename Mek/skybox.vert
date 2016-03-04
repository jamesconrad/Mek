#version 430

layout (location = 0) in vec3 position;
out vec3 uvw;

uniform mat4 m;
vec4 p;

void main()
{
	p = m * vec4(position, 1.0);
	gl_Position = p.xyzw;
	uvw = position;
}