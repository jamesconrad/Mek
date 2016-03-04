#include "FramebufferEffects.h"

FramebufferEffects::FramebufferEffects(Framebuffer* fb, Framebuffer* wb0, Framebuffer* wb1)
{
	_fb = fb;
	_wb[0] = wb0;
	_wb[1] = wb1;

	_size = glm::vec2(_wb[0]->Width(), _wb[0]->Height());
	pixsizeX = 1.f / (_size.x);
	pixsizeY = 1.f / (_size.y);
}

FramebufferEffects::FramebufferEffects(Framebuffer* fbwbwb[4])
{
	_fb = fbwbwb[0];
	_wb[0] = fbwbwb[1];
	_wb[1] = fbwbwb[2];
	_wb[2] = fbwbwb[3];

	_size = glm::vec2(_wb[0]->Width(), _wb[0]->Height());
	pixsizeX = 1.f / (_size.x / 2.f);
	pixsizeY = 1.f / (_size.y / 2.f);
}

void FramebufferEffects::LoadBloomShaders()
{
	Program::getInstance().createShader("tonemap", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("tonemap", GL_FRAGMENT_SHADER, "shaders/tonemap.frag");
	Program::getInstance().createShader("gblurH", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("gblurH", GL_FRAGMENT_SHADER, "shaders/gblurH.frag");
	Program::getInstance().createShader("gblurV", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("gblurV", GL_FRAGMENT_SHADER, "shaders/gblurV.frag");
	Program::getInstance().createShader("combine", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("combine", GL_FRAGMENT_SHADER, "shaders/combine.frag");
}

void FramebufferEffects::LoadFXAAShaders()
{
	Program::getInstance().createShader("fxaa", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("fxaa", GL_FRAGMENT_SHADER, "shaders/fxaa.frag");
}

void FramebufferEffects::LoadShadowMapShaders()
{

}

void FramebufferEffects::loadToonShaders()
{
	Program::getInstance().createShader("toon", GL_VERTEX_SHADER, "shaders/pass.vert");
	Program::getInstance().createShader("toon", GL_FRAGMENT_SHADER, "shaders/toon.frag");
}

void FramebufferEffects::Bloom(unsigned int numGaussPasses)
{
	//_fb->Bind();
	////_wb[0]->Clear();
	//Program::getInstance().bind("tonemap");
	//_fb->PassTextureToPreBoundShader("tex", 0);
	//_fb->RenderQuad();
	//Framebuffer::Unbind();
	_wb[0]->Bind();
	//_wb[0]->Clear();
	glViewport(0, 0, _size.x, _size.y);
	Program::getInstance().bind("tonemap");
	_fb->PassTextureToPreBoundShader("tex", 0);
	_wb[0]->RenderQuad();
	
	for (int i = 0; i < numGaussPasses; i++)
	{
		_wb[1]->Bind();
		//_wb[1]->Clear();
		Program::getInstance().bind("gblurH");
		Program::getInstance().setUniform("pixsize", pixsizeX);
		_wb[0]->PassTextureToPreBoundShader("tex", 0);
		_wb[1]->RenderQuad();
	
		_wb[0]->Bind();
		//_wb[0]->Clear();
		Program::getInstance().bind("gblurV");
		Program::getInstance().setUniform("pixsize", pixsizeY);
		_wb[1]->PassTextureToPreBoundShader("tex", 0);
		_wb[0]->RenderQuad();
	}
	glViewport(0, 0, _size.x * 2, _size.y * 2);
	_fb->Bind();
	Program::getInstance().bind("combine");
	_fb->PassTextureToPreBoundShader("texa", 0);
	_wb[0]->PassTextureToPreBoundShader("texb", 0);
	_fb->RenderQuad();
}

void FramebufferEffects::FXAA()
{
	_fb->Bind();
	Program::getInstance().bind("fxaa");
	Program::getInstance().setUniform("screensize", glm::vec2(_fb->Width(), _fb->Height()));
	_fb->PassTextureToPreBoundShader("tex", 0);
	_fb->RenderQuad();
}

void FramebufferEffects::PrepShadowMap()
{

}

void FramebufferEffects::FinShadowMap()
{

}

void FramebufferEffects::Toon(bool doCraziness)
{
	_wb[2]->Bind();

	Program::getInstance().bind("toon");
	_fb->PassTextureToPreBoundShader("rgbTexture", 0);
	_fb->PassTextureToPreBoundShader("depthTexture", 1);
	_fb->PassTextureToPreBoundShader("normalTexture", 2);
	if (doCraziness)
	{
		Program::getInstance().setUniform("fboSize", glm::vec3(0));
		Program::getInstance().setUniform("sobelCorrection", 0);
		Program::getInstance().setUniform("magnitudeCorrection", 1);
		Program::getInstance().setUniform("magnitudeFix", 1);
	}
	else
	{
		Program::getInstance().setUniform("fboSize", glm::vec3(_wb[2]->Width(), _wb[2]->Height(), 0.0f));
		Program::getInstance().setUniform("sobelCorrection", 1);
		Program::getInstance().setUniform("magnitudeCorrection", -1);
		Program::getInstance().setUniform("magnitudeFix", 0);
	}
	_wb[2]->RenderQuad();

	_fb->Bind();
	Program::getInstance().bind("pass");
	_wb[2]->PassTextureToPreBoundShader("tex0", 0);
	_fb->RenderQuad();

}									  
