#pragma once
#include "Interpolation.h"
#include "ComponentCollision.h"
#include "ComponentGraphics.h"
#include "AIBehaviour.h"
#include "NavMesh.h"
#include "RayVsOBB.h"

#include "lib\glm\gtx\vector_angle.hpp"

class Target
{
public:
	//Must manually set the interpolation vars through the public accessor
	Target(char* fp, float tmod);
	//should be called after the collision check
	void update(float dTime, NavMesh &_mesh);
	Interpolation interp;
	bool hit;
	bool alive;
	float tmod;
	GameObject* go;
	ComponentGraphics* cg;
	ComponentCollision* cc;

	AiBehaviour AiHandle;
	NavFace tempPosition; //I don't like this, but it's my best idea atm. I'll try and improve this when it's not 4 am.

	//Resets the path for the object
	void generatePath(NavMesh &navMesh);

	void generatePath(NavMesh &navMesh, glm::vec3 &endPosition);
	

	bool hasSpottedPlayer = false;
	float distanceToPlayer, angleToPlayer, angleTolerance = 120.0f, distanceTolerance = 15.f;
	glm::vec3 vectorToPlayer;
	bool canSeePlayer(glm::vec3 &playerPosition);

	void determineCombatRoute(NavMesh &navMesh);
};