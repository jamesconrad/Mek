#version 430 core

layout(location = 0) in vec2 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;
out float interpT;

uniform float _min;
uniform float _max;

void main()
{
	gl_Position =  vec4(vertexPosition.xy,0,1);
	
	UV = vertexUV;
	interpT = (vertexPosition.y - _min) / (_max - _min);
}