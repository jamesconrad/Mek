#version 330

layout (location = 0) in vec3 Position;

uniform mat4 gWVP;//aligns to camera
out float h;

void main()
{
    gl_Position  = gWVP * vec4(Position, 1);
	h = Position.y;
}