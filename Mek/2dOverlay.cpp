#include "2dOverlay.h"
#include "Camera.h"
#include "lib\glm\gtc\matrix_transform.hpp"


static const float vertices[] = {
	//vert pos   // tex coord
	 -0.5f, -0.5f, 0.0f, 0.0f, 
	  0.5f, -0.5f, 1.0f, 0.0f, 
	  0.5f,  0.5f, 1.0f, 1.0f, 
	 -0.5f,  0.5f, 0.0f, 1.0f
};
static const unsigned int indicies[] = { 0, 2, 1, 0, 3, 2 };

void twodOverlay::cutoffPercent(float c)
{
	cutoff = c;
}

twodOverlay::twodOverlay(char* fp, float x, float y, float scale)
{
	_tex = new Texture(fp, false);

	pos = glm::vec3(x, y, scale);
	cutoff = 1;

	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	glGenBuffers(2, _vbo);

	//Setup vertex data POSITION = 0
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 16 * sizeof(GLfloat), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//Setup index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indicies, GL_STATIC_DRAW);


	glBindVertexArray(0);
}

void twodOverlay::render()
{
	Program::getInstance().use("hud");
	//glDisable(GL_DEPTH_TEST);
	glBindVertexArray(_vao);
	
	Program::getInstance().setUniform("transformationdata", pos);
	Program::getInstance().setUniform("cutoff", cutoff);
	Program::getInstance().setUniform("projMatrix", Camera::getInstance().projection());
	//Program::getInstance().setUniform("hud", "proj", glm::ortho(0.f, 1920.f, 1080.f, 0.f) * Camera::getInstance().view());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex->object());

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	//glEnable(GL_DEPTH_TEST);
	Program::getInstance().stopUsing("hud");
}