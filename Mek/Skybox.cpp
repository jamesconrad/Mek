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

Skybox::Skybox(char* fp[6], char* op[6], char* sp[6])
{
	Program::getInstance().createShader("skybox", GL_VERTEX_SHADER, "shaders/skybox.vert");
	Program::getInstance().createShader("skybox", GL_FRAGMENT_SHADER, "shaders/skybox.frag");

	Program::getInstance().createShader("skybox2", GL_VERTEX_SHADER, "shaders/skybox.vert");
	Program::getInstance().createShader("skybox2", GL_FRAGMENT_SHADER, "shaders/skybox2.frag");

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);

	glGenBuffers(1, &_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(float), cubeVertices, GL_STATIC_DRAW);

	int farr[6];
	int ofarr[6];
	int sfarr[6];

	_bmp[0].bitmapFromFile(fp[0], &farr[0]);
	_bmp[1].bitmapFromFile(fp[1], &farr[1]);
	_bmp[2].bitmapFromFile(fp[2], &farr[2]);
	_bmp[2].rotate90CounterClockwise();
	_bmp[3].bitmapFromFile(fp[3], &farr[3]);
	_bmp[3].rotate90CounterClockwise();
	_bmp[3].rotate90CounterClockwise();
	_bmp[3].rotate90CounterClockwise();
	_bmp[4].bitmapFromFile(fp[4], &farr[4]);
	_bmp[5].bitmapFromFile(fp[5], &farr[5]);

	
	_obscureBMP[0].bitmapFromFile(op[0], &ofarr[0]);
	_obscureBMP[1].bitmapFromFile(op[1], &ofarr[1]);
	_obscureBMP[2].bitmapFromFile(op[2], &ofarr[2]);
	_obscureBMP[2].rotate90CounterClockwise();
	_obscureBMP[3].bitmapFromFile(op[3], &ofarr[3]);
	_obscureBMP[3].rotate90CounterClockwise();
	_obscureBMP[3].rotate90CounterClockwise();
	_obscureBMP[3].rotate90CounterClockwise();
	_obscureBMP[4].bitmapFromFile(op[4], &ofarr[4]);
	_obscureBMP[5].bitmapFromFile(op[5], &ofarr[5]);

	_staticBMP[0].bitmapFromFile(sp[0], &sfarr[0]);
	_staticBMP[1].bitmapFromFile(sp[1], &sfarr[1]);
	_staticBMP[2].bitmapFromFile(sp[2], &sfarr[2]);
	_staticBMP[2].rotate90CounterClockwise();
	_staticBMP[3].bitmapFromFile(sp[3], &sfarr[3]);
	_staticBMP[3].rotate90CounterClockwise();
	_staticBMP[3].rotate90CounterClockwise();
	_staticBMP[3].rotate90CounterClockwise();
	_staticBMP[4].bitmapFromFile(sp[4], &sfarr[4]);
	_staticBMP[5].bitmapFromFile(sp[5], &sfarr[5]);


	glGenTextures(1, &_cubeMapID);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);


	for (int i = 0; i < 6; i++)
	{
		//int mode;
		//if (farr[i] == 4)
		//	mode = GL_RGBA;
		//else
		//	mode = GL_RGB;
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, _bmp[i].width(), _bmp[i].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _bmp[i].pixelBuffer());
		//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mode, 128, 128, 0, mode, GL_UNSIGNED_BYTE, _bmp[i].pixelBuffer());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glGenTextures(1, &_obscureMapID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _obscureMapID);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, _obscureBMP[i].width(), _obscureBMP[i].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _obscureBMP[i].pixelBuffer());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindVertexArray(0);


	glGenTextures(1, &_staticMapID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _staticMapID);

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, _staticBMP[i].width(), _staticBMP[i].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, _staticBMP[i].pixelBuffer());
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	glBindVertexArray(0);
}

void Skybox::render(bool ObscurityMap)
{
	//update shader vars
	
	Program::getInstance().use("skybox");
	
	
	glBindVertexArray(_vao);

	glm::mat4 skyMap = glm::translate(Camera::getInstance().projection() * Camera::getInstance().view(), Camera::getInstance().position());

	Program::getInstance().setUniform("m", skyMap);
	if (ObscurityMap)
		Program::getInstance().setUniform("doStatic", glm::vec3(0));
	else
		Program::getInstance().setUniform("doStatic", glm::vec3(1));

	glDepthMask(GL_FALSE);
	//glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapID);
	Program::getInstance().setUniform("skybox", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _obscureMapID);
	Program::getInstance().setUniform("obsbox", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _staticMapID);
	Program::getInstance().setUniform("stabox", 2);
	//glBindBuffer(GL_ARRAY_BUFFER, _vbo[i]);
	glDrawArrays(GL_TRIANGLES, 0, 108);
	glBindVertexArray(0);

	//glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
}