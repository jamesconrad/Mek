#version 330

layout(location = 0) in vec3 vert;

uniform mat4 camera;
uniform mat4 model;
uniform vec3 colour;
out vec4 color;

void main() {
    // Apply all matrix transformations to vert
	color = vec4(colour, 1);
    gl_Position = camera * model * vec4(vert, 1);
}