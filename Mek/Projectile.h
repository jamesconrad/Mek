#pragma once

#include "Model.h"
#include "ComponentCollision.h"
#include "FSoundManager.h"

extern float timeFactor;
extern bool isUsingBulletTime;

class Projectile
{
public:
	
	Projectile(glm::vec3 p, glm::vec3 d, float v, float _d, float lifespan,FSound*);
	
	void update(float &dtime, bool _isUsingBulletTime = false)
	{
		pos += dir * (_isUsingBulletTime ? (vel * dtime) * timeFactor : (vel * dtime));
		go->pos = pos;
		sound->soundPos = { pos.x, pos.y, pos.z };
		life -= dtime;
		ChannelPtr->isPlaying(&isSound);
		if (!isSound)
			ChannelPtr->stop();

		if (life < 0){
			alive = false;
			ChannelPtr->stop();
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
	FMOD::Channel* ChannelPtr;
	GameObject* go;
	ComponentCollision* cc;
	Model* cg;
};

void prepProjectiles();