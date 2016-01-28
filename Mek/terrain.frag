#version 330

uniform sampler2D t0;
uniform sampler2D t1;

out vec4 colour;
in vec3 p;
in vec3 n;
in float h;


void main()
{
	vec2 uv;
	uv.x = abs(p.x) - floor(abs(p.x));
	uv.y = abs(p.z) - floor(abs(p.z));
	vec4 c0 = texture(t0, uv);
	vec4 c1 = texture(t1, uv);
	colour = mix(c0, c1, h);
	//colour = vec4(n, 1);
}