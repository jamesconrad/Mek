#version 430

in vec2 uv;
out vec4 fragColour;
out vec4 tex1Colour;
out vec4 tex2Colour;
out vec4 tex3Colour;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;


void main()
{
	fragColour = texture2D(tex0, uv);
	tex1Colour = texture2D(tex1, uv);
	tex2Colour = texture2D(tex2, uv);
	tex3Colour = texture2D(tex3, uv);
}