#include "Target.h"

Target::Target(char* fp, float t)
{
	tmod = t;
	hit = false;
	alive = true;
	go = new GameObject(0);
	go->SetName("Target");
	cg = new ComponentGraphics();
	cg->setOwner(go);
	cg->loadModel(fp);
	cc = new ComponentCollision();
	cc->setCollisionMask(cg->getScene());
	cc->setOwner(go);
}

void Target::update(float dTime)
{
	if (!hit && go->scale == glm::vec3(0, 0, 0))
	{
		hit = true;
	}
	else
	{
		interp.speedControlInterp(dTime * tmod);
		go->pos = interp.pos;
	}
}