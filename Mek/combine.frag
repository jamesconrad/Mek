#version 430

in vec2 uv;
layout(location = 0) out vec4 colour;

uniform sampler2D texa;
uniform sampler2D texb;

void main()
{
	colour = 1 - ((1 - texture2D(texa, uv)) * (1 - texture2D(texb, uv)));
	//colour = texture2D(texa, uv) + texture2D(texb, uv);
	//colour = texture2D(texa, uv);
}