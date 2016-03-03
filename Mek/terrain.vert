#version 430

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
	gl_Position = gWVP * vec4(Position , 1);

	p = Position;
	h = (Position.y - _min) / (_max - _min);
	n = Normal;
}