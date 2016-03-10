#pragma once
#include "Interpolation.h"
#include "ComponentCollision.h"
#include "Model.h"
#include "AIBehaviour.h"
#include "NavMesh.h"
#include "RayVsOBB.h"
#include "Projectile.h"

#include "SoundManager.h"

#include "lib\glm\gtx\vector_angle.hpp"



enum combatPositionType
{
	OFFENSIVE,
	DEFENSIVE
};
struct combatPosition
{
	glm::vec3 position;
	glm::vec2 positionCoordinatesInTriangleSet;
	float distanceToPlayer;
	combatPositionType positionType;
};


class Target
{
public:
	//Must manually set the interpolation vars through the public accessor
	Target(char* fp, float tmod, OwnerList* _list);
	//should be called after the collision check
	void update(float dTime, NavMesh &_mesh);
	Interpolation interp;
	bool hit;
	bool alive;
	float tmod;
	GameObject* go;
	Model* cg;
	ComponentCollision* cc;

	OwnerList* oList;

	AiBehaviour AiHandle;
	NavFace tempPosition; //I don't like this, but it's my best idea atm. I'll try and improve this when it's not 4 am.
	NavFace currentFace;

	//Resets the path for the object
	void generatePath(NavMesh &navMesh);

	void generatePath(NavMesh &navMesh, glm::vec3 &endPosition);
	

	bool hasSpottedPlayer = false;
	float distanceToPlayer, angleToPlayer, angleTolerance = 60.0f, distanceTolerance = 15.0f;
	glm::vec3 vectorToPlayer;
	bool canSeePlayer(glm::vec3 &playerPosition);

	void determineCombatRoute(NavMesh &navMesh);



	//definitions needed for sampling of points for followPath:
	//The ai agent's definisions
	combatPosition tcp;
	glm::vec3 tcpPosition; //temporaryCombatPosition 
	glm::vec2 tcpCoordinatesInTriangleSet;
	float tcpDistanceToPlayer;
	combatPositionType tcpPositionType;
	glm::vec3 vecToPlayer;
	//possible positions
	std::vector<combatPosition> surroundingPositions;
	combatPosition bestOffensivePosition;
	//Object we are comparing to.
	GameObject *comparisonObjectGO;
	ComponentCollision *comparisonObjectCC;
	void followPath(float &deltaTime, NavMesh &mesh, bool &doCombat);


	glm::vec3 currentTargetNode;
	int currentNodeIterator = 0;

	float maxVelocity = 10.0f;
	float pointRadius = 3.0f;
	float steeringCorrectionFactor = 8.0f;
	glm::vec3 currentVelocity = { 1.f, 0.f, 0.f };
	glm::vec3 desiredVelocity;
	glm::vec3 steering;

	bool selectedToDoCombatUpdate = false;



	Projectile 
		*weaponProjectile;
	float fireTimer = 0.f;
	float fireTimeTolerance = 2.0f;
	FSound *laserSound;
	FMOD::Channel* movingsound;
};

