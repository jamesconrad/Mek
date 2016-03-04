#version 330

uniform sampler2D terrainG0;
uniform sampler2D terrainG1;
uniform sampler2D terrainG2;
uniform sampler2D terrainG3;
//uniform vec3 u;
//uniform vec3 d;

out vec4 colour;
in vec3 p;
in vec3 n;
in float h;

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
		//scale0 = dn - trange3;
		//scale1 = 1 - scale0;
		//colour = texture2D(terrainG1, uv) * scale0;
		//colour += texture2D(terrainG2, uv) * scale1;
		//colour = vec4(0,1,0,1);
		colour = mix(texture2D(terrainG1, uv), texture2D(terrainG2, uv), (dn - trange2) / (trange3 - trange2));
	}
	else if (dn <= trange4) 
		colour = texture2D(terrainG2, uv);
	else if (dn <= trange5)
	{
		//scale0 = dn - trange5;
		//scale1 = 1 - scale0;
		//colour = texture2D(terrainG2, uv) * scale0;
		//colour += texture2D(terrainG3, uv) * scale1;
		//colour = vec4(0,0,1,1);
		colour = mix(texture2D(terrainG2, uv), texture2D(terrainG3, uv), (dn - trange4) / (trange5 - trange4));
	}
	else
		colour = texture2D(terrainG3, uv);

	//colour = vec4(dn,0,0, 1);
	//colour = (texture2D(terrainG0, uv) + texture2D(terrainG1, uv) + texture2D(terrainG2, uv) + texture2D(terrainG3, uv)) * 0.25;
	//colour = texture2D(terrainG2, uv);
	//lets just use the y val to determine the texture

	//colour = vec4(h,0,0,1);

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
}