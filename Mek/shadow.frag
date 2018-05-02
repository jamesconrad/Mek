#version 330

layout(location = 0) out vec4 fragmentdepth;
layout(location = 1) out vec4 depth;
layout(location = 2) out vec4 normals;
layout(location = 3) out vec4 lightObscurers;
in float zcoord;
in vec3 vertNormal;

void main(){
	fragmentdepth = vec4(vec3(zcoord), 1);
	depth = vec4(vec3(gl_FragCoord.z), 1.0);
	normals = vec4(vertNormal, 1.0);
	lightObscurers = vec4(1, 0, 0, 1);

}