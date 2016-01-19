#version 330

layout (location = 0) in vec3 position;
out vec3 uv;

uniform mat4 camera;

void main()
{
	gl_Position = mat3(camera) * (position * 500);
	uv = position;
}