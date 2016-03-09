#version 430

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;

uniform mat4 depthMVP; //aligns to sun
out float zcoord;

void main()
{
	gl_Position = depthMVP * vec4(Position , 1);
	zcoord = gl_Position.z;
}