#pragma once

#include "ComponentGraphics.h"
#include "ComponentCollision.h"
#include "FMODmanager.h"

class Projectile
{
public:
	
	Projectile(glm::vec3 p, glm::vec3 d, float v, float _d, float lifespan,FMOD::Channel*);
	
	void update(float dtime)
	{
		pos += dir * (vel);
		go->pos = pos;
		FMOD_VECTOR sp = { pos.x, pos.y, pos.z };
		FMOD_VECTOR sv = { 0.0, 0.0, 0.0 };
		sound->set3DAttributes(&sp, &sv);
		life -= dtime;
		sound->isPlaying(&isSound);
		if (!isSound)
			sound->stop();

		if (life < 0){
			alive = false;
			sound->stop();
		}
	}

	glm::vec3 pos, dir;
	float vel;
	float dmg;
	float life;
	int handle;
	//unsigned double soundLength;
	bool isSound;
	bool alive;
	FMOD::Channel* sound;

	GameObject* go;
	ComponentCollision* cc;
	ComponentGraphics* cg;
};

void prepProjectiles();