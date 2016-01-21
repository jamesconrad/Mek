#include "Framebuffer.h"

GLuint Framebuffer::ScreenQuadVAO = 0;
GLuint Framebuffer::ScreenQuadVBO = 0;

static const GLfloat quadVBData[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};

Framebuffer::Framebuffer()
{
	glGenFramebuffers(1, &_fbo);
	//check if screen quad has been made
	if (!ScreenQuadVAO)
	{
		glGenVertexArrays(1, &ScreenQuadVAO);
		glBindVertexArray(ScreenQuadVAO);
		glGenBuffers(1, &ScreenQuadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, ScreenQuadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVBData), quadVBData, GL_STATIC_DRAW);
	}
}

void Framebuffer::CreateColorTexture(unsigned int width, unsigned int height)
{
	Bind();
	GLuint cbo;
	glGenTextures(1, &cbo);
	glBindTexture(GL_TEXTURE_2D, cbo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + _cbo.size(), GL_TEXTURE_2D, cbo, 0);
	_cbo.push_back(cbo);
	_buf.push_back(GL_COLOR_ATTACHMENT0 + _cbo.size());
	Unbind();
}

void Framebuffer::CreateDepthTexture(unsigned int width, unsigned int height)
{
	Bind();
	glGenRenderbuffers(1, &_dbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _dbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _dbo);
	Unbind();
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glDrawBuffers(_buf.size(), _buf.data());
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDrawBuffer(0);
}

bool Framebuffer::Check()
{
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return false;
	return true;
}

void Framebuffer::Render()
{

}