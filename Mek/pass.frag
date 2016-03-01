#version 430

in vec2 uv;
out vec4 fragColour;
uniform sampler2D tex0;

void main()
{
	fragColour = texture2D(tex0, uv);
}