#version 330

layout (location = 0) in vec4 Position;

uniform vec3 transformationdata;

out vec2 texCoord;

void main()
{
	texCoord = Position.zw;
	
	vec3 pos = vec3(Position.xy, 0);
	pos.x /= 16;
	pos.y /= 9;
	pos *= transformationdata.z;
	pos.x += transformationdata.x;
	pos.y += transformationdata.y;

	gl_Position = vec4(pos.xy, 0, 1);
}