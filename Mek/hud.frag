#version 330

in vec2 texCoord;
uniform sampler2D tex;

out vec4 fragcolour;

void main()
{
	//fragcolour = vec4(texCoord.x, texCoord.y, 0, 1);
	fragcolour = texture(tex, texCoord.xy);
}