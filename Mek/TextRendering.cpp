#include <vector>
#include <cstring>
#include "TextRendering.h"

#include "Texture.h"
#include "Program.h"

#include "lib\glm\glm.hpp"
#include "lib\glm\gtc\matrix_transform.hpp"

unsigned int Text2DTextureID;
unsigned int Text2DVertexBufferID;
unsigned int Text2DUVBufferID;
unsigned int Text2DShaderID;
unsigned int Text2DUniformID;
unsigned int vao;

void TextRendering::initText2D(char * texturePath){

	// Initialize texture
	Texture * textureID = new Texture(texturePath, false);
	Text2DTextureID = textureID->object();

	// Initialize VBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &Text2DVertexBufferID);
	glGenBuffers(1, &Text2DUVBufferID);

	// Initialize Shader
	//Text2DShaderID = LoadShaders( "TextVertexShader.vertexshader", "TextVertexShader.fragmentshader" );
	Program::getInstance().createShader("text", GL_VERTEX_SHADER, "shaders/text.vert");
	Program::getInstance().createShader("text", GL_FRAGMENT_SHADER, "shaders/text.frag");
	Text2DShaderID = Program::getInstance().object("text");
	// Initialize uniforms' IDs
	Text2DUniformID = glGetUniformLocation(Text2DShaderID, "myTextureSampler");

}

void TextRendering::printText2D(const char * text, float x, float y, float size, glm::vec3 colourT, glm::vec3 colourB)
{
	if (colourB == glm::vec3(-1, -1, -1))
		colourB = colourT;
	
	unsigned int length = strlen(text);

	// Fill buffers
	std::vector<glm::vec2> vertices;
	std::vector<glm::vec2> UVs;
	for (unsigned int i = 0; i < length; i++)
	{
		float isize = (float)i * size;
		glm::vec2 vertex_up_left = glm::vec2(x + isize, y + size);
		glm::vec2 vertex_up_right = glm::vec2(x + isize + size, y + size);
		glm::vec2 vertex_down_right = glm::vec2(x + isize + size, y);
		glm::vec2 vertex_down_left = glm::vec2(x + isize, y);

		vertices.push_back(vertex_up_left);
		vertices.push_back(vertex_down_left);
		vertices.push_back(vertex_up_right);

		vertices.push_back(vertex_down_right);
		vertices.push_back(vertex_up_right);
		vertices.push_back(vertex_down_left);

		char character = text[i] - 32;
		float uv_x = (character % 8) / 8.0f;
		float uv_y = (character / 8) / 8.0f;

		glm::vec2 uv_up_left = glm::vec2(uv_x, uv_y);
		glm::vec2 uv_up_right = glm::vec2(uv_x + 1.0f / 8.0f, uv_y);
		glm::vec2 uv_down_right = glm::vec2(uv_x + 1.0f / 8.0f, (uv_y + 1.0f / 8.0f));
		glm::vec2 uv_down_left = glm::vec2(uv_x, (uv_y + 1.0f / 8.0f));
		UVs.push_back(uv_up_left);
		UVs.push_back(uv_down_left);
		UVs.push_back(uv_up_right);

		UVs.push_back(uv_down_right);
		UVs.push_back(uv_up_right);
		UVs.push_back(uv_down_left);
	}

	glBindVertexArray(vao);
	// Bind texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Text2DTextureID);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	//glUniform1i(Text2DUniformID, Text2DTextureID);
	Program::getInstance().bind("text");
	Program::getInstance().setUniform("colT", colourT);
	Program::getInstance().setUniform("colB", colourB);
	Program::getInstance().setUniform("_min", y);
	Program::getInstance().setUniform("_max", y + size);


	// 1rst attribute buffer : vertices
	glBindBuffer(GL_ARRAY_BUFFER, Text2DVertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec2), &vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	// 2nd attribute buffer : UVs
	glBindBuffer(GL_ARRAY_BUFFER, Text2DUVBufferID);
	glEnableVertexAttribArray(1);
	glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Draw call
	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
}

void TextRendering::cleanupText2D(){

	// Delete buffers
	glDeleteBuffers(1, &Text2DVertexBufferID);
	glDeleteBuffers(1, &Text2DUVBufferID);

	// Delete texture
	glDeleteTextures(1, &Text2DTextureID);

	// Delete shader
	glDeleteProgram(Text2DShaderID);
}

