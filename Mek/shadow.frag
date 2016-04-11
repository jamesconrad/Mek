#version 330

layout(location = 0) out vec4 cascade0;
layout(location = 1) out vec4 depth;
layout(location = 2) out vec4 normals;
layout(location = 3) out vec4 lightObscurers;
layout(location = 4) out vec4 cascade1;
layout(location = 5) out vec4 cascade2;

in float zcoord0;
in float zcoord1;
in float zcoord2;
in vec3 vertNormal;

void main()
{
	cascade0 = vec4(vec3(zcoord0), 1);
	cascade1 = vec4(vec3(zcoord1), 1);
	cascade2 = vec4(vec3(zcoord2), 1);
	depth = vec4(vec3(gl_FragCoord.z), 1.0);
	normals = vec4(vertNormal, 1.0);
	lightObscurers = vec4(1, 0, 0, 1);

}