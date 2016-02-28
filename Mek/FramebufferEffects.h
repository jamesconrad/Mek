#pragma once

#include "Framebuffer.h"
#include "Program.h"

class FramebufferEffects
{
public:
	//For both constructors, the first element is the fullscreen framebuffer that that will hold the reuslt after the effect,
	//the other two elements are half the size and used as work buffers
	FramebufferEffects(Framebuffer* fb, Framebuffer* wb0, Framebuffer* wb1);
	FramebufferEffects(Framebuffer* fbwbwb[3]);

	void LoadBloomShaders();
	void LoadFXAAShaders();

	void Bloom(unsigned int numGaussPasses);
	void FXAA();

private:
	Framebuffer* _fb;
	Framebuffer* _wb[2];
	glm::vec2 _size;
	float pixsizeX;
	float pixsizeY;
};