#version 330

layout(location = 0) out vec4 fragmentdepth;
in float zcoord;

void main(){
	fragmentdepth = vec4(vec3(zcoord), 1);
}