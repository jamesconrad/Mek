#version 430

// Vertex Shader Inputs
// These are the attributes of the vertex
layout(location = 0) in vec3 vIn_vertex;
layout(location = 1) in vec3 vIn_normal;
layout(location = 2) in vec3 vIn_uv;
layout(location = 3) in vec4 vIn_colour;

// Uniforms
// Constants throughout the entire pipeline
// These values are sent from C++ (glSendUniform*)
uniform mat4 u_mvp;
uniform mat4 u_mv;


out VertexData
{
	vec3 vertex;
	vec3 normal;
	vec3 texCoord;
	vec4 colour;
} vOut;

out vec4 FragColour;
void main()
{
	// Pass attributes through pipeline
	//vOut.vertex = vec3(u_mv * vec4(vIn_vertex, 1.0));
	//vOut.texCoord = vIn_uv;
	//vOut.colour = vIn_colour;
	//vOut.normal = (u_mv * vec4(vIn_normal, 0.0)).xyz;


	
	// Pass attributes through pipeline
	vOut.texCoord = vIn_uv;
	vOut.colour = vIn_colour;
	vOut.normal = (u_mv * vec4(vIn_normal, 0.0)).xyz;

	gl_Position = u_mvp * vec4(vIn_vertex, 1.0);
}