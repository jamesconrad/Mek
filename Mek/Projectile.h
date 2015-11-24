#pragma once

#include "ComponentGraphics.h"
#include "ComponentCollision.h"

class Projectile
{
public:
	
	Projectile(glm::vec3 p, glm::vec3 d, float v, float _d, float lifespan);
	
	void update(float dtime)
	{
		pos += dir * (vel);
		go->pos = pos;
		life -= dtime;
		if (life < 0)
			alive = false;
	}

	glm::vec3 pos, dir;
	float vel;
	float dmg;
	float life;
	int handle;

	bool alive;

	GameObject* go;
	ComponentCollision* cc;
	ComponentGraphics* cg;
};

void prepProjectiles();