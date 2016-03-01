#version 430

in vec2 uv;
layout(location = 0) out vec4 colour;

uniform sampler2D tex;
uniform float pixsize;

void main()
{
	colour.rgb = vec3(0.0, 0.0, 0.0);
	colour.rgb += texture2D(tex, vec2(uv.x - 4.0 * pixsize, uv.y)).rgb * 0.06;
	colour.rgb += texture2D(tex, vec2(uv.x - 3.0 * pixsize, uv.y)).rgb * 0.09;
	colour.rgb += texture2D(tex, vec2(uv.x - 2.0 * pixsize, uv.y)).rgb * 0.12;
	colour.rgb += texture2D(tex, vec2(uv.x -       pixsize, uv.y)).rgb * 0.15;
	colour.rgb += texture2D(tex, vec2(uv.x                , uv.y)).rgb * 0.16;
	colour.rgb += texture2D(tex, vec2(uv.x +       pixsize, uv.y)).rgb * 0.15;
	colour.rgb += texture2D(tex, vec2(uv.x + 2.0 * pixsize, uv.y)).rgb * 0.12;
	colour.rgb += texture2D(tex, vec2(uv.x + 3.0 * pixsize, uv.y)).rgb * 0.09;
	colour.rgb += texture2D(tex, vec2(uv.x + 4.0 * pixsize, uv.y)).rgb * 0.06;
	colour.a = 1.0;
}

