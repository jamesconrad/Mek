#version 430

uniform sampler2D t0;
uniform sampler2D t1;
//uniform vec3 u;
//uniform vec3 d;

layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 Depth;
layout(location = 2) out vec4 Normal;

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
	//colour = mix(c0, c1, h) * 0.5;
	//colour = vec4(n, 1);

	// new slope based method
	//i mean were adding 10 hours to the
	//load time by calculating it.. lets use it

	float s = dot(vec3(0,1,0), normalize(n));
	colour = mix(c0, c1, h);
	//if (p.x < 0.1 && p.x > -0.1)
	//	colour.g = 1;
	//if (p.z < 0.1 && p.z > -0.1)
	//	colour.r = 1;
	//if (uv.x < 0.05 && uv.x > -0.05)
	//	colour.b = 0.1;
	//if (uv.y < 0.05 && uv.x > -0.05)
	//	colour.b = 0.1;
	//
	//if (p.x > d.x && p.x < u.x)
	//	if (p.z > u.z && p.z < d.z)
	//		colour.r = 1;
	//colour = vec4(normalize(n) * 0.5 + 0.5, 1);

	Depth = vec4(vec3(gl_FragCoord.z), 1.0);
	//Depth = vec4(0, 0, 0, 1.0);
	Normal = vec4(vec3(n), 1.0);
	//Normal = vec4(0, 0, 0, 1.0);
}