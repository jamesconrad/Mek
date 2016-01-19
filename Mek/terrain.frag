#version 330

uniform float _min;
uniform float _max;

out vec4 colour;
in float h;

void main()
{
	float a = (h / _max) + _min;
	colour = mix(vec4(0,0,1,1), vec4(1,0,0,1), a);
	//colour = vec4(_max * 10000,0,0,1);
}