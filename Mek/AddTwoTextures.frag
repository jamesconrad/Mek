#version 430

in vec2 uv;
out vec4 fragColour;

uniform sampler2D tex0;
uniform sampler2D tex1;


void main()
{
	fragColour = texture2D(tex0, uv) + texture2D(tex1, uv);
}