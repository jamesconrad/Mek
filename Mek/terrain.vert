#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 gWVP;//aligns to camera
uniform mat4 depthMVP; //aligns to sun
out vec3 n;
out vec3 p;
out float h;
out vec4 fpls;

uniform mat4 identity = mat4(1,0,0,0,
							 0,1,0,0,
							 0,0,1,0,
							 0,0,0,1);

uniform float _min;
uniform float _max;

out float vertDepth;

void main()
{
	gl_Position = gWVP * vec4(Position , 1);

	
	fpls = depthMVP * vec4(Position, 1.0);

	p = Position;
	h = (Position.y - _min) / (_max - _min);
	n = Normal;

	vertDepth = gl_Position.z;
}