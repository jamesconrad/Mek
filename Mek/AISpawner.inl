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
			targets[i]->go->scale = glm::vec3(1, 1, 1);
			targets[i]->go->pos = position;
			targets[i]->hasSpottedPlayer = false;
			//targets[i]->update(0.166f, testNaveMesh);
			targets[i]->hit = false;
			targets[i]->alive = true;
			targets[i]->go->health = 100.f;
			return;
		}
	}
}

inline void TargetSpawner::update(std::vector<Target *> &targets, float &deltaTime)
{
	spawnTimer -= deltaTime;
	if (spawnTimer <= 0)
	{
		spawnNewTarget(targets);
		spawnTimer = randomClampedFloat(3.0f, 5.0f);
	}
}