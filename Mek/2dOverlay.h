#pragma once

#include "Program.h"
#include "Texture.h"

class twodOverlay
{
public:
	twodOverlay(char* fp, float x, float y, float s);

	void render();

	//THE Z value represents the overall scale
	glm::vec3 pos;
private:
	unsigned int _vao;
	unsigned int _vbo[2];
	
	Texture* _tex;
};