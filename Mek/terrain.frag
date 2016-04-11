#version 330

uniform sampler2D terrainG0;
uniform sampler2D terrainG1;
uniform sampler2D terrainG2;
uniform sampler2D terrainG3;
uniform sampler2D shadowMap0;
uniform sampler2D shadowMap1;
uniform sampler2D shadowMap2;
uniform float clipEnd0;
uniform float clipEnd1;
uniform float clipEnd2;
//uniform vec3 u;
//uniform vec3 d;

layout(location = 0) out vec4 colour;
layout(location = 1) out vec4 depth;
layout(location = 2) out vec4 normals;
layout(location = 3) out vec4 LightObscurers;

in vec3 p;
in vec3 n;
in float h;

in vec4 fpls[3];
in float clipZ;
in float vertDepth;

void main()
{
	vec2 uv;
	uv.x = abs(p.x) - floor(abs(p.x));
	uv.y = abs(p.z) - floor(abs(p.z));

	float dn = dot(normalize(n), vec3(0,1,0));
	dn = dn * dn * dn * dn * dn * dn * dn * dn * dn;

	float trange0 = 0.15;
	float trange1 = 0.30;
	float trange2 = 0.45;
	float trange3 = 0.60;
	float trange4 = 0.75;
	float trange5 = 0.90;
	
	float scale0, scale1;
	if (dn >= 0.0 && dn <= trange0) 
		colour = texture2D(terrainG0, uv);
	else if (dn <= trange1)
		colour = mix(texture2D(terrainG0, uv), texture2D(terrainG1, uv), (dn - trange0) / (trange1 - trange0));
	else if (dn <= trange2) 
		colour = texture2D(terrainG1, uv);
	else if (dn <= trange3)
	{
		colour = mix(texture2D(terrainG1, uv), texture2D(terrainG2, uv), (dn - trange2) / (trange3 - trange2));
	}
	else if (dn <= trange4) 
		colour = texture2D(terrainG2, uv);
	else if (dn <= trange5)
	{
		colour = mix(texture2D(terrainG2, uv), texture2D(terrainG3, uv), (dn - trange4) / (trange5 - trange4));
	}
	else
		colour = texture2D(terrainG3, uv);

	//The following is for enabling the terrain grid for determining distances
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
	int cascade;
	if (clipZ < clipEnd0)
	{
		cascade = 0;
		vec3 proj = fpls[cascade].xyz / fpls[cascade].w;
		uv.x = 0.5 * proj.x + 0.5; //done with uv above so lets steal it
		uv.y = 0.5 * proj.y + 0.5;
		float z = 0.5 * proj.z + 0.5;

		float depth = texture2D(shadowMap0, fpls[cascade].xy).r;

		if (depth < z + 0.00001)
			colour.xyz = colour.xyz * 0.25;
		colour.r += 0.5;
	}
	else if (clipZ < clipEnd1)
	{
		cascade = 1;
		vec3 proj = fpls[cascade].xyz / fpls[cascade].w;
		uv.x = 0.5 * proj.x + 0.5; //done with uv above so lets steal it
		uv.y = 0.5 * proj.y + 0.5;
		float z = 0.5 * proj.z + 0.5;

		float depth = texture2D(shadowMap0, fpls[cascade].xy).r;

		if (depth < z + 0.00001)
			colour.xyz = colour.xyz * 0.25;
	}
	else if (clipZ < clipEnd2)
	{
		cascade = 2;
		vec3 proj = fpls[cascade].xyz / fpls[cascade].w;
		uv.x = 0.5 * proj.x + 0.5; //done with uv above so lets steal it
		uv.y = 0.5 * proj.y + 0.5;
		float z = 0.5 * proj.z + 0.5;

		float depth = texture2D(shadowMap0, fpls[cascade].xy).r;

		if (depth < z + 0.00001)
			colour.xyz = colour.xyz * 0.25;
		colour.b += 0.5;
	}
	else
	{
		colour.xyz = vec3(1,0,0);
	}

	depth = vec4(vec3(0), 1.0);
	normals = vec4(vec3(n), 1.0);
	LightObscurers = vec4(0.0, 0.0, 0.0, 1.0);
}