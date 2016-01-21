#include "lib\glm\gtc\matrix_transform.hpp"
#include "Skybox.h"
#include "Program.h"

GLfloat cubeVertices[] = {
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	
	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,
	
	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,
	
	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

Skybox::Skybox(char* fp[6])
{
	Program::getInstance().createShader("skybox", GL_VERTEX_SHADER, "shaders/skybox.vert");
	Program::getInstance().createShader("skybox", GL_FRAGMENT_SHADER, "shaders/skybox.frag");


	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	_bmp[0].bitmapFromFile(fp[0]);
	_bmp[1].bitmapFromFile(fp[1]);
	_bmp[2].bitmapFromFile(fp[2]);
	_bmp[2].rotate90CounterClockwise();
	_bmp[3].bitmapFromFile(fp[3]);
	_bmp[3].rotate90CounterClockwise();
	_bmp[4].bitmapFromFile(fp[4]);
	_bmp[5].bitmapFromFile(fp[5]);

	glGenTextures(1, &_cubeMapID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, _bmp[i].width(), _bmp[i].height(), 0, GL_RGB, GL_UNSIGNED_BYTE, _bmp[i].pixelBuffer());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindVertexArray(0);
}

void Skybox::render()
{
	//update shader vars
	Program::getInstance().use("skybox");

	glBindVertexArray(_vao);

	glm::mat4 skyMap = glm::translate(Camera::getInstance().projection() * Camera::getInstance().view(), Camera::getInstance().position());

	Program::getInstance().setUniform("skybox", "m", skyMap);

	glDepthMask(GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);
	//glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);
	glDrawArrays(GL_TRIANGLES, 0, 108);
	glBindVertexArray(0);

	glDepthMask(GL_TRUE);
}