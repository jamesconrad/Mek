#include "Target.h"

Target::Target(char* fp)
{
	hit = false;
	alive = true;
	go = new GameObject(0);
	go->SetName("Target");
	cg = new ComponentGraphics();
	cg->loadModel(fp);
	cc = new ComponentCollision();
	cc->setCollisionMask(cg->getScene());
}

void Target::update(float dTime)
{
	if (!hit && go->scale == glm::vec3(0, 0, 0))
	{
		hit = true;
	}
	else
	{
		interp.interpolate(dTime);
		go->pos = interp.pos;
	}
}