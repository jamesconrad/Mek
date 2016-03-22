#pragma once

#include <string>
#include <iostream>
#include "include\GL\glew.h"
#include "Texture.h"
#include "Bitmap.h"
#include "lib\glm\glm.hpp"

class TextRendering
{
public:
	static TextRendering& getInstance()
	{
		static TextRendering instance;
		return instance;
	}
	
	//Member Functions
	void initText2D( char * texturePath);
	void printText2D(const char * text, float x, float y, float size, glm::vec3 colourTop, glm::vec3 colourBottom = glm::vec3(-1,-1,-1));
	void cleanupText2D();
	void renderText();

private:
	TextRendering() {};
	Texture * textureID;
};