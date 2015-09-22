#include <string>
#include <iostream>
#include "include\GL\glew.h"

class TextRendering
{
public:
	void initText2D(const char * texturePath);
	void printText2D(const char * text, int x, int y, int size);
	void cleanupText2D();
};