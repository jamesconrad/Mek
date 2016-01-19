in vec3 uv;
out vec4 colour;

uniform samplerCube skybox;

void main()
{
	colour = texture(skybox, uv);
}