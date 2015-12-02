#version 330 core

// Interpolated values from the vertex shaders
in vec2 UV;

// Ouput data
out vec4 color;

uniform vec3 textcol;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main()
{
	color = texture( myTextureSampler, UV );

	if (color.xyz != vec3(1,1,1))
		color.w = 0;
	else
		color.xyz *= textcol;
}