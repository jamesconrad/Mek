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

	void Bind();
	void BindForDraw();
	static void Unbind();
	static void UnbindForDraw();

	void Render(char * shader);

private:
	GLuint _fbo;
	GLuint _dbo;
	std::vector<GLuint> _cbo;
	std::vector<GLuint> _buf;
	//unsigned int _numColor;

	static GLuint ScreenQuadVAO;
	static GLuint ScreenQuadVBO;
};