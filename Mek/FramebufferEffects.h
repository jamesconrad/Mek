#pragma once

#include "Framebuffer.h"
#include "Program.h"

class FramebufferEffects
{
public:
	//For both constructors, the first element is the fullscreen framebuffer that that will hold the reuslt after the effect,
	//the other two elements are half the size and used as work buffers
	FramebufferEffects(Framebuffer* fb, Framebuffer* wb0, Framebuffer* wb1);
	FramebufferEffects(Framebuffer* fbwbwb[4]);

	void LoadBloomShaders();
	void LoadFXAAShaders();
	void LoadShadowMapShaders();
	void loadToonShaders();

	void Bloom(unsigned int numGaussPasses);
	void FXAA();
	void Toon();

	void PrepShadowMap();
	void FinShadowMap();

private:
	Framebuffer* _fb;
	Framebuffer* _wb[3];
	glm::vec2 _size;
	float pixsizeX;
	float pixsizeY;
};