#version 330

layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoord;
layout (location = 2) in vec3 Normal;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

uniform mat4 LVP0;
uniform mat4 LVP1;
uniform mat4 LVP2;

out float zcoord0;
out float zcoord1;
out float zcoord2;
out vec3 vertNormal;

void main()
{
    gl_Position  = LVP0 * vec4(Position, 1.0);
	vertNormal = Normal;
	zcoord0 = (LVP0 * vec4(Position, 1.0)).z;
	zcoord1 = (LVP1 * vec4(Position, 1.0)).z;
	zcoord2 = (LVP2 * vec4(Position, 1.0)).z;
}