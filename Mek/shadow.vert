#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

uniform mat4 LVP;

out float zcoord;
out vec3 vertNormal;

void main()
{
    gl_Position  = LVP * vec4(Position, 1.0);
	vertNormal = Normal;
	zcoord = gl_Position.z;
}