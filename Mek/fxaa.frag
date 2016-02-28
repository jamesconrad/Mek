#version 430
//1.f / 128.f
#define FXAA_REDUCE_MUL 0.0078125
//1.f / 8.f
#define FXAA_REDUCE_MIN 0.125
#define FXAA_SPAN_MAX 8.0
in vec2 uv;
layout (location = 0) out vec4 colour;
uniform sampler2D tex;

uniform vec2 pixsize;
uniform vec2 screensize;

void main()
{
	colour = texture2D(tex, uv);
}