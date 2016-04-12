#pragma once
#include "AISpawner.h"

inline void TargetSpawner::activate()
{
	isActive = true;
}

inline void TargetSpawner::deactivate()
{
	isActive = false;
}

inline void TargetSpawner::spawnNewTarget(std::vector<Target *> &targets)
{
	for (unsigned int i = 0, s = targets.size(); i < s; i++)
	{
		if (targets[i]->alive == false)
		{
			if (randomClampedFloat(0, 2) > 1.f)
			{
				targets[i]->enemyType = STANDARD;
				targets[i]->maxVelocity = 12.f;
				targets[i]->go->health = 100.f;
			}
			else
			{
				targets[i]->enemyType = HEAVYHITTER;
				targets[i]->maxVelocity = 5.0f;
				targets[i]->go->health = 175.f;
			}
			targets[i]->go->scale = glm::vec3(1, 1, 1);
			targets[i]->go->pos = position;
			targets[i]->hasSpottedPlayer = false;
			//targets[i]->update(0.166f, testNaveMesh);
			targets[i]->hit = false;
			targets[i]->alive = true;
			return;
		}
	}
}

inline void TargetSpawner::update(std::vector<Target *> &targets, float deltaTime)
{
	spawnTimer -= deltaTime;
	if (spawnTimer <= 0)
	{
		spawnNewTarget(targets);
		spawnTimer = randomClampedFloat(2.0f, 3.0f);
	}
}