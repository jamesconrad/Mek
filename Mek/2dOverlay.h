#pragma once

#include "Program.h"
#include "Texture.h"

class twodOverlay
{
public:
	twodOverlay(char* fp, float x, float y, float s);

	void render();

	//takes a decimal from 0-1 representing the percentage to show
	void cutoffPercent(float cutoff);

	//THE Z value represents the overall scale
	glm::vec3 pos;
private:
	unsigned int _vao;
	unsigned int _vbo[2];

	float cutoff;
	
	Texture* _tex;
};