#pragma once
#include "Interpolation.h"
#include "ComponentCollision.h"
#include "ComponentGraphics.h"

class Target
{
public:
	//Must manually set the interpolation vars through the public accessor
	Target(char* fp);
	//should be called after the collision check
	void update(float dTime);
	Interpolation interp;
	bool hit;
	bool alive;
	GameObject* go;
	ComponentGraphics* cg;
	ComponentCollision* cc;
};