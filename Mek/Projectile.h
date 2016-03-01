#pragma once

#include "ComponentGraphics.h"
#include "ComponentCollision.h"
#include "SoundManager.h"

class Projectile
{
public:
	
	Projectile(glm::vec3 p, glm::vec3 d, float v, float _d, float lifespan,FSound*);
	
	void update(float &dtime)
	{
		pos += dir * (vel);
		go->pos = pos;
		FMOD_VECTOR sp = { pos.x, pos.y, pos.z };
		FMOD_VECTOR sv = { 0.0, 0.0, 0.0 };
		sound->ChannelPtr->set3DAttributes(&sp, &sv);
		life -= dtime;
		sound->ChannelPtr->isPlaying(&isSound);
		if (!isSound)
			sound->ChannelPtr->stop();

		if (life < 0){
			alive = false;
			sound->ChannelPtr->stop();
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
	FSound* sound;

	GameObject* go;
	ComponentCollision* cc;
	ComponentGraphics* cg;
};

void prepProjectiles();