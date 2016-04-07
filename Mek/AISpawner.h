#pragma once
#include "Target.h"
#include "AIBehaviour.h"
#include "lib\glm\glm.hpp"

class TargetSpawner
{
public:
	TargetSpawner(){};
	~TargetSpawner(){};
	inline void activate();
	inline void deactivate();
	inline void update(std::vector<Target *> &targetsVector, float deltaTime);

public:
	bool isActive = false;
	float spawnTimer = 0.0f;
	float maxSpawnTimer = 3.0f;
	glm::vec3 position = glm::vec3(0.f, 0.f, -47.f * 0.1f);

private:
	inline void spawnNewTarget(std::vector<Target *> &targetsVector);
};

float maxTargetNumberIncreaseTimer = 10.f;
float TargetNumberIncreaseTime = maxTargetNumberIncreaseTimer;

#include "AISpawner.inl"