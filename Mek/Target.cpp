#include "Target.h"

Target::Target(char* fp, char* hvyfp, float t, SoundList* _oList)
{
	tmod = t;
	hit = false;
	alive = true;
	go = new GameObject(0);
	go->SetName("Target");
	cg = new Model();
	cg->setOwner(go);
	cg->loadModel(fp);
	cc = new ComponentCollision();
	cc->setOwner(go);
	cc->setCollisionMask(cg->getScene());
	cc->createHitboxRender();
	cc->type = STATIC;

	Component* c = cg;
	go->AddComponent(GRAPHICS, c);
	c = cc;
	go->AddComponent(PHYSICS, c);

	hvygo = new GameObject(0);
	hvygo->SetName("Target");
	hvycg = new Model();
	hvycg->setOwner(hvygo);
	hvycg->loadModel(hvyfp);
	hvycc = new ComponentCollision();
	hvycc->setOwner(hvygo);
	hvycc->setCollisionMask(hvycg->getScene());
	hvycc->createHitboxRender();
	hvycc->type = STATIC;

	hvygo->AddComponent(GRAPHICS, hvycg);
	hvygo->AddComponent(PHYSICS, hvycc);
	hvygo->scale = glm::vec3(1.3f);

	healthBar = new GameObject(0);
	healthBar->SetName("HealthBar");
	healthBarCG = new Model();
	healthBarCG->setOwner(healthBar);
	healthBarCG->loadModel("models/HealthBar.dae");
	healthBar->AddComponent(GRAPHICS, healthBarCG);

	oList = _oList;
	//oList->PlayAndPauseAll();
	//movingsound = (oList->FindSound("Moving"))->Play();
	//oList->FindSound("Moving")->Pause();
	ObjectManager::instance().addObject(go);

	go->dir = glm::vec3(1.0f, 0.0f, 0.0f);
	go->vel = 2.f;

	surroundingPositions.reserve(11);
	
}

void Target::update(float dTime, NavMesh &mesh)
{
	//oList->UpdateOwnerSoundPos(go->pos);

	if (!hit && go->health <= 0) //go->scale == glm::vec3(0, 0, 0))
	{
		//hit = true;
		//oList->FindAndPlay("Hit");
	}
	else
	{
		glm::vec2 tempCoords = mesh.isPointInsideTriangle(go->pos);
		if (tempCoords.x > -1 && tempCoords.y > -1)
		{
			currentFace = mesh.TriangleSet[tempCoords.x][tempCoords.y];
		}

		go->dir = go->pos;// glm::normalize(interp.pos - go->dir);
		followPath(dTime, mesh, hasSpottedPlayer);
		go->dir = glm::normalize(go->pos - go->dir);

		glm::vec3 pPos = go->pos;
		FMOD_VECTOR fpos{ pPos.x, pPos.y, pPos.z };
		FMOD_VECTOR vel{ 0, 0, 0 };
		movingsound->set3DAttributes(&fpos, &vel);
		glm::vec3 npos = go->pos;
		glm::vec3 temp = glm::vec3((npos.x), (npos.y), (npos.z));
		glm::vec3 temp2 = glm::vec3((pPos.x), (pPos.y), (pPos.z));

		if (enemyType == STANDARD)
		{
			leftGunBarrel = glm::vec3(cg->rotMatrix * glm::vec4(-0.4f, 0.93f, -0.4f, 1.0f));
			rightGunBarrel = glm::vec3(cg->rotMatrix * glm::vec4(-0.4f, 0.93f, 0.4f, 1.0f));
			healthBarCG->rotMatrix = cg->rotMatrix;
			healthBarCG->cut = go->health / 100.f;
		}
		else if (enemyType == HEAVYHITTER)
		{
			leftGunBarrel = glm::vec3(hvycg->rotMatrix * glm::vec4(-0.4f, 1.09f, -0.7f, 1.0f));
			rightGunBarrel = glm::vec3(hvycg->rotMatrix * glm::vec4(-0.4f, 1.09f, 0.7f, 1.0f));
			healthBarCG->rotMatrix = hvycg->rotMatrix;
			healthBarCG->cut = go->health / 175.f;
		}
		//std::cout << "go->pos: " << go->pos.x << " " << go->pos.y << " " << go->pos.z << std::endl;
		oList->UpdateListPosition(go->pos);
		hvygo->pos = go->pos;
		hvygo->dir = go->dir;
		//movingsound->setPaused(false);
		//if ((oList->FindSound("Moving")->GetDistanceToSystem()) > (oList->FindSound("Moving"))->GetMaxDistance()){
		//	oList->FindSound("Moving")->GetSoundChannelPtr()->setPaused(true);
		//}
		//else{
		//	oList->FindSound("Moving")->GetSoundChannelPtr()->setPaused(false);
		//}
	}
	
}

//Resets and creates a path for the target
void Target::generatePath(NavMesh &mesh)
{
	currentNodeIterator = 0;
	interp.points.clear();
	float clampedNumberX1 = randomClampedInt(0, mesh.TriangleSet.size() - 1);					float clampedNumberX2 = randomClampedInt(0, mesh.TriangleSet.size() - 1);
	float clampedNumberY1 = randomClampedInt(0, mesh.TriangleSet[clampedNumberX1].size() - 1);  float clampedNumberY2 = randomClampedInt(0, mesh.TriangleSet[clampedNumberX2].size() - 1);
	//glm::vec2 temp = mesh.isPointInsideTriangle(go->pos); //assuming that the object is staring inside the nav mesh
	AiHandle.determineRoute(mesh, tempPosition, mesh.TriangleSet[clampedNumberX2][clampedNumberY2]);
	for (int i = AiHandle.path.size() - 1; i >= 0; i--)
	{
		interp.points.push_back(AiHandle.path[i]->center + glm::vec3(0, 0.4, 0));
	}
	//interp.curve.clear();
	//interp.buildCurve();
}

void Target::generatePath(NavMesh &mesh, glm::vec3 &endPosition)
{
	currentNodeIterator = 0;
	glm::vec2 coords = mesh.isPointInsideTriangle(endPosition);
	AiHandle.determineRoute(mesh, tempPosition, mesh.TriangleSet[coords.x][coords.y]);
	for (int i = AiHandle.path.size() - 1; i >= 0; i--)
	{
		interp.points.push_back(AiHandle.path[i]->center + glm::vec3(0, 0.4, 0));
	}
	//interp.curve.clear();
	//interp.buildCurve();
}

bool Target::canSeePlayer(glm::vec3 &position)
{
	distanceToPlayer = glm::distance(go->pos, position);
	vectorToPlayer = glm::normalize(glm::vec3(position - go->pos));
	angleToPlayer = glm::angle(vectorToPlayer, go->dir);
	if (angleToPlayer <= angleTolerance && distanceToPlayer < distanceTolerance)
	{
		hasSpottedPlayer = true;
		AiHandle.currentBehaviour = FIGHTING;
	}
	return hasSpottedPlayer;
}

void Target::determineCombatRoute(NavMesh &navMesh)
{
	//Sample surroundings:
	
	//getting the player's position:
	glm::vec3 playerPosition = ObjectManager::instance().gMap[0]->pos; //This is actually the player.
	vecToPlayer = glm::normalize(playerPosition - tcpPosition);

	//set current position variables:
	tcpCoordinatesInTriangleSet = navMesh.isTriangleInsideTriangle(currentFace);
	tcpPosition = navMesh.TriangleSet[tcpCoordinatesInTriangleSet.x][tcpCoordinatesInTriangleSet.y].center;
	tcpDistanceToPlayer = glm::distance(tcpPosition, playerPosition);
	tcpPositionType = OFFENSIVE; //The position is only defensive if proven to be so.
	
	//check if there is an object between the position and the player.
	for (int i = 1, s = ObjectManager::instance().colMap.size(); i < s; i++)
	{
		comparisonObjectGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[i]]);
		comparisonObjectCC = static_cast<ComponentCollision*>(comparisonObjectGO->GetComponent(PHYSICS));
		if (RayVsOBB(tcpPosition, vectorToPlayer, comparisonObjectCC->_cMesh[0]->fmin, comparisonObjectCC->_cMesh[0]->fmax))
		{
			//If there is an object between the player and the length of the origin to the first colliding point is less than the distance to the player, the position is defensive.
			//currently not actually used. I'll need to figure out a way to do the above. I think I do, but I'm not quite sure yet.
			tcpPositionType = DEFENSIVE;
			break;
		}
	}
	//push_back the position into surroundingPositions.
	tcp.position = tcpPosition;
	tcp.positionCoordinatesInTriangleSet = tcpCoordinatesInTriangleSet;
	tcp.distanceToPlayer = tcpDistanceToPlayer;
	tcp.positionType = tcpPositionType;
	surroundingPositions.push_back(tcp);
	bestOffensivePosition = surroundingPositions.back();



	//Check the points that actually surround the first point.
	for (int i = 1; i < 5; i++)
	{
		//Generate new position
		glm::vec3 addedVector = glm::normalize(glm::rotateY(glm::vec3(0, 0, 1), 89.9f * i)) * 5.0f;
		tcpPosition = (surroundingPositions[0].position) + addedVector;

		//Check if the point is inside the mesh
		tcpCoordinatesInTriangleSet = navMesh.isPointInsideTriangle(tcpPosition);
		if (tcpCoordinatesInTriangleSet.x == -1 || tcpCoordinatesInTriangleSet.y == -1)
		{
			continue;
		}
		
		//set the variables for the position being examined.
		vecToPlayer = glm::normalize(playerPosition - tcpPosition);
		tcpDistanceToPlayer = glm::distance(playerPosition, tcpPosition);
		tcpPositionType = OFFENSIVE;

		//Check the position against the collision map.
		for (int i = 1, s = ObjectManager::instance().colMap.size(); i < s; i++)
		{
			comparisonObjectGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[i]]);
			comparisonObjectCC = static_cast<ComponentCollision*>(comparisonObjectGO->GetComponent(PHYSICS));
			if (RayVsOBB(tcpPosition, vectorToPlayer, comparisonObjectCC->_cMesh[0]->fmin, comparisonObjectCC->_cMesh[0]->fmax))
			{
				//If there is an object between the player and the length of the origin to the first colliding point is less than the distance to the player, the position is defensive.
				tcpPositionType = DEFENSIVE;
				break;
			}
		}
		//push_back the new position
		tcp.position = tcpPosition;
		tcp.positionCoordinatesInTriangleSet = tcpCoordinatesInTriangleSet;
		tcp.distanceToPlayer = tcpDistanceToPlayer;
		tcp.positionType = tcpPositionType;
		surroundingPositions.push_back(tcp);
		if (tcp.distanceToPlayer < bestOffensivePosition.distanceToPlayer)
		{
			bestOffensivePosition = tcp;
		}
	}

	//reset tempPosition to the current position
	tempPosition = navMesh.TriangleSet[surroundingPositions[0].positionCoordinatesInTriangleSet.x][surroundingPositions[0].positionCoordinatesInTriangleSet.y];

	//generate the new curve.
	interp.points.clear();
	generatePath(navMesh, bestOffensivePosition.position);
	//generatePath(navMesh, playerPosition);
	surroundingPositions.clear();
}

void Target::followPath(float &dT, NavMesh &mesh, bool &doCombat)
{
	if (glm::distance(glm::vec3(go->pos.x, 0.496205002, go->pos.z), currentTargetNode) < pointRadius)
	{
		currentNodeIterator++;
		unsigned int lolsize = interp.points.size() - 1;
		if (currentNodeIterator >= interp.points.size() - 1)
		{
			currentNodeIterator = 0;
			//create new path.
			if (doCombat)
			{
				determineCombatRoute(mesh);
				selectedToDoCombatUpdate = false;
			}
			else
			{
				generatePath(mesh);
			}
			currentTargetNode = interp.points[0];
		}
	}
	currentTargetNode = interp.points[currentNodeIterator];
	

	desiredVelocity = glm::normalize(currentTargetNode - glm::vec3(go->pos.x, 0.496205002, go->pos.z)) * currentMaxVelocity;
	steering = desiredVelocity - currentVelocity;
	currentVelocity = glm::normalize(currentVelocity + steering / steeringCorrectionFactor) * currentMaxVelocity + go->force;
	go->force = go->force / 1.2f;

	go->pos = go->pos + currentVelocity * dT;
}