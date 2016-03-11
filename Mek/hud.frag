#version 430

in vec2 texCoord;
uniform sampler2D tex;
uniform float cutoff;

out vec4 fragcolour;

void main()
{
	//fragcolour = vec4(texCoord.x, texCoord.y, 0, 1);
	if (texCoord.x > cutoff)
		fragcolour = vec4(0);
	else
	{
		fragcolour = texture(tex, texCoord.xy);
		fragcolour *= 0.9;
	}
}