#version 430

in vec2 TexCoord0;
in vec3 Normal0;
in vec3 WorldPos0;

in vec4 Debug0;

out vec4 FragColor;

void main()
{
	FragColor = vec4(Debug0.x,Debug0.y,Debug0.z,1);
}