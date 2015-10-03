#include <string>
#include <iostream>
#include "include\GL\glew.h"
#include "Texture.h"
#include "Bitmap.h"

class TextRendering
{
public:

	//constructor
	TextRendering();
	TextRendering(char * filename)
	{
		initText2D(filename);
	}

	//Destructor
	~TextRendering();

	//Member Functions
	void initText2D( char * texturePath);
	void printText2D(const char * text, int x, int y, int size);
	void cleanupText2D();
	void renderText();

private:
	Texture * textureID;
	
};