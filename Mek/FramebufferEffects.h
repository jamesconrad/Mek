#pragma once

#include "Framebuffer.h"
#include "Program.h"
#include "lib\glm\gtc\matrix_transform.hpp"

class FramebufferEffects
{
public:
	//For both constructors, the first element is the fullscreen framebuffer that that will hold the reuslt after the effect,
	//the other two elements are half the size and used as work buffers
	FramebufferEffects(Framebuffer* fb, Framebuffer* wb0, Framebuffer* wb1);
	FramebufferEffects(Framebuffer* fbwbwb[5]);

	void LoadBloomShaders();
	void LoadFXAAShaders();
	void LoadShadowMapShaders();
	void loadToonShaders();
	void LoadGodRayShaders();

	void Bloom(unsigned int numGaussPasses);
	void FXAA();
	void Toon(bool doCraziness);
	void GodRays(glm::vec3 &sunLocation, glm::vec3 &playerLocation, glm::vec3 &cameraForward);

	void PrepShadowMap();
	int FinShadowMap();

private:
	Framebuffer* _fb;
	Framebuffer* _wb[4];
	glm::vec2 _size;
	float pixsizeX;
	float pixsizeY;
};