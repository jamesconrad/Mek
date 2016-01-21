#include "include\GL\glew.h"
#include <vector>
#include "Program.h"

//Integrate with the program class with the binds
//In shader class will need to create an "attatchFramebuffer" class

class Framebuffer
{
public:
	Framebuffer();

	void CreateDepthTexture(unsigned int width, unsigned int height);
	void CreateColorTexture(unsigned int width, unsigned int height);
	bool Check();

	void Bind();
	static void Unbind();

	void Render();

private:
	GLuint _fbo;
	GLuint _dbo;
	std::vector<GLuint> _cbo;
	std::vector<GLuint> _buf;
	//unsigned int _numColor;

	static GLuint ScreenQuadVAO;
	static GLuint ScreenQuadVBO;
};