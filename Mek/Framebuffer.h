#pragma once

#include "include\GL\glew.h"
#include <vector>
#include "Program.h"

//Integrate with the program class with the binds
//In shader class will need to create an "attatchFramebuffer" class

class Framebuffer
{
public:
	Framebuffer();

	void CreateDepthTexture(unsigned int width = 1920, unsigned int height = 1080);
	void CreateColorTexture(unsigned int num, unsigned int width = 1920, unsigned int = 1080);
	bool Check();

	void BindTextureForSampling(int texIndex, GLenum texUnit);
	void UnbindTexture(GLenum texUnit);

	unsigned int Width();
	unsigned int Height();

	void Bind();
	static void Unbind();

	void Clear();

	void Render(char * shader);
	void RenderQuad();
	void PassTextureToPreBoundShader(char* uniform, int cbo);

private:
	GLuint _fbo;
	GLuint _dbo;
	std::vector<GLuint> _cbo;
	std::vector<GLuint> _buf;
	//unsigned int _numColor;

	unsigned int _w, _h;

	static GLuint ScreenQuadVAO;
	static GLuint ScreenQuadVBO;
	static unsigned int Texture;
};