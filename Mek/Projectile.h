#pragma once

#include "Model.h"
#include "ComponentCollision.h"
#include <SlimManager\Sound.h>
extern float timeFactor;
extern bool isUsingBulletTime;

class Projectile
{
public:
	
	Projectile(glm::vec3 p, glm::vec3 d, float v, float _d, float lifespan,Sound*);
	
	void update(float &dtime, bool _isUsingBulletTime = true)
	{
		//pos += dir * (_isUsingBulletTime ? ((0.0000000000000000f) * dtime) * (timeFactor) : (vel * dtime));
		if (_isUsingBulletTime)
		{
			pos += dir * (timeFactor + 0.7f) * dtime;
		}
		else
		{
			pos += dir * vel * dtime;
		}
		go->pos = pos;
		sound->SetSoundPosition(pos);
		life -= dtime;
		ChannelPtr->isPlaying(&isSound);
		//std::cout << isSound;
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
	Sound* sound;
	FMOD::Channel* ChannelPtr;
	GameObject* go;
	ComponentCollision* cc;
	Model* cg;
};

void prepProjectiles();