#include "Target.h"

Target::Target(char* fp, float t)
{
	tmod = t;
	hit = false;
	alive = true;
	go = new GameObject(0);
	go->SetName("Target");
	cg = new ComponentGraphics();
	cg->setOwner(go);
	cg->loadModel(fp);
	cc = new ComponentCollision();
	cc->setCollisionMask(cg->getScene());
	cc->setOwner(go);
	cc->createHitboxRender();
	cc->type = STATIC;

	Component* c = cg;
	go->AddComponent(GRAPHICS, c);
	c = cc;
	go->AddComponent(PHYSICS, c);
	 
	ObjectManager::instance().addObject(go);

	go->dir = glm::vec3(1.0f, 0.0f, 0.0f);
	go->vel = 2.f;
}

void Target::update(float dTime, NavMesh &mesh)
{
	if (!hit && go->scale == glm::vec3(0, 0, 0))
	{
		hit = true;
	}
	else
	{
		if (AiHandle.currentBehaviour == INTERPOLATING)
		{
			interp.speedControlInterp(dTime * tmod);
			go->dir = glm::normalize(interp.pos - go->dir);
			go->pos = interp.pos;
			if (interp.isFinished)
			{
				tempPosition = *AiHandle.path.front();
				interp.points.clear();
				generatePath(mesh);
				//go->pos = AiHandle.path.back()->center;
			}
		}
		else if (AiHandle.currentBehaviour == WANDERING)
		{
			go->dir = AiHandle.update(go->pos, go->dir, dTime);
			go->pos = go->pos + go->dir * go->vel * dTime;
		}
		else if (AiHandle.currentBehaviour == FIGHTING)
		{
			if (interp.isFinished)
			{
				//do nothing
			}
			else
			{
				interp.speedControlInterp(dTime * tmod);
				go->dir = glm::normalize(interp.pos - go->dir);
				go->pos = interp.pos;
			}
		}
	}
	
}

//Resets and creates a path for the target
void Target::generatePath(NavMesh &mesh)
{
	float clampedNumberX1 = randomClampedInt(0, mesh.TriangleSet.size() - 1);					float clampedNumberX2 = randomClampedInt(0, mesh.TriangleSet.size() - 1);
	float clampedNumberY1 = randomClampedInt(0, mesh.TriangleSet[clampedNumberX1].size() - 1);  float clampedNumberY2 = randomClampedInt(0, mesh.TriangleSet[clampedNumberX2].size() - 1);
	//glm::vec2 temp = mesh.isPointInsideTriangle(go->pos); //assuming that the object is staring inside the nav mesh
	AiHandle.determineRoute(mesh, tempPosition, mesh.TriangleSet[clampedNumberX2][clampedNumberY2]);
	for (int i = AiHandle.path.size() - 1; i >= 0; i--)
	{
		interp.points.push_back(AiHandle.path[i]->center + glm::vec3(0, 0.4, 0));
	}
	interp.curve.clear();
	interp.buildCurve();
}

void Target::generatePath(NavMesh &mesh, glm::vec3 &endPosition)
{
	glm::vec2 coords = mesh.isPointInsideTriangle(endPosition);
	AiHandle.determineRoute(mesh, tempPosition, mesh.TriangleSet[coords.x][coords.y]);
	for (int i = AiHandle.path.size() - 1; i >= 0; i--)
	{
		interp.points.push_back(AiHandle.path[i]->center + glm::vec3(0, 0.4, 0));
	}
	interp.curve.clear();
	interp.buildCurve();
}

bool Target::canSeePlayer(glm::vec3 &position)
{
	distanceToPlayer = glm::distance(go->pos, position);
	vectorToPlayer = glm::normalize(glm::vec3(go->pos - position));
	angleToPlayer = glm::angle(vectorToPlayer, go->dir);
	if (angleToPlayer <= angleTolerance && distanceToPlayer < distanceTolerance)
		hasSpottedPlayer = true;
	AiHandle.currentBehaviour = FIGHTING;
	return hasSpottedPlayer;
}

	enum combatPositionType
	{
		OFFENSIVE,
		DEFENSIVE
	};
struct combatPosition
{
	glm::vec3 position;
	float distanceToPlayer;
	combatPositionType positionType;
};

void Target::determineCombatRoute(NavMesh &navMesh)
{
	glm::vec2 pointCoordinates;
	std::vector<combatPosition> positions;
	combatPosition* bestOffensivePosition;
	combatPosition* bestDefensivePosition;

	combatPosition tempPosition;
	tempPosition.position = navMesh.TriangleSet[pointCoordinates.x][pointCoordinates.y].center;
	tempPosition.distanceToPlayer = LONG_MAX;
	bestDefensivePosition = &tempPosition;
	bestOffensivePosition = &tempPosition;
	for (int i = 0; i < 10; i++)
	{
		pointCoordinates = navMesh.isPointInsideTriangle(glm::vec3(glm::rotateY(glm::vec4(go->dir, 1.0f), 160.f / i) * 30.f * ((float) (i % 2) + 0.5f)));
		if (pointCoordinates.x != -1 && pointCoordinates.y != -1)
		{
			for (int i = 1, s = ObjectManager::instance().colMap.size(); i < s; i++)
			{
				GameObject* thisGO = static_cast<GameObject*>(ObjectManager::instance().gMap[ObjectManager::instance().colMap[i]]);
				ComponentCollision* thisCC = static_cast<ComponentCollision*>(thisGO->GetComponent(PHYSICS));
				tempPosition.distanceToPlayer = glm::distance(tempPosition.position, ObjectManager::instance().gMap[0]->pos); //gmap[0] is the player.
				if (RayVsOBB(thisCC->getPos(), vectorToPlayer, thisCC->_cMesh[0]->fmin, thisCC->_cMesh[0]->fmax))
				{
					tempPosition.distanceToPlayer = glm::distance(tempPosition.position, thisCC->_cMesh[0]->fc);
					if (tempPosition.distanceToPlayer < distanceToPlayer)
					{
						tempPosition.positionType = DEFENSIVE;
						positions.push_back(tempPosition);
						if (tempPosition.distanceToPlayer > bestDefensivePosition->distanceToPlayer)
						{
							bestDefensivePosition = &positions.back();
						}
						break;
					}
				}
				tempPosition.positionType = OFFENSIVE;
				positions.push_back(tempPosition);
				if (tempPosition.distanceToPlayer < bestOffensivePosition->distanceToPlayer)
				{
					bestOffensivePosition = &positions.back();
				}
			}
		}
	}
	//Add in some stuff to change whether the ai chooses a defensive or offensive position based on health or something. Stuff like that.
	interp.points.clear();
	generatePath(navMesh, bestOffensivePosition->position);
}
