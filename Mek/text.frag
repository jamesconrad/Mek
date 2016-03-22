#version 430 core

// Interpolated values from the vertex shaders
in vec2 UV;
in float interpT;
// Ouput data
out vec4 color;

uniform vec3 colT;
uniform vec3 colB;

// Values that stay constant for the whole mesh.
uniform sampler2D myTextureSampler;

void main()
{
	color = texture( myTextureSampler, UV );

	if (color.xyz != vec3(1,1,1))
		color.w = 0;
	else
		color.xyz = mix(colB, colT, interpT);
}