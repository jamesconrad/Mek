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

void Target::canSeePlayer(glm::vec3 &position)
{
	vectorToPlayer = glm::normalize(glm::vec3(go->pos - position));
	angleToPlayer = glm::angle(vectorToPlayer, go->dir);
	if (angleToPlayer <= angleTolerance)
		hasSpottedPlayer = true;
}

	enum combatPositionType
	{
		ATTACKING,
		DEFENDING
	};
struct combatPosition
{
	glm::vec3 position;
	combatPositionType positionType;
};

void Target::determineCombatRoute(NavMesh &navMesh)
{
	glm::vec2 pointCoordinates;
	std::vector<combatPosition> positions;
	for (float i = 0; i < 10; i++)
	{
		pointCoordinates = navMesh.isPointInsideTriangle(glm::vec3(glm::rotateY(glm::vec4(go->dir, 1.0f), 160.f / i) * 10.f * i % 2.f));
		if (pointCoordinates.x != -1 && pointCoordinates.y != -1)
		{
			combatPosition tempPosition;
			tempPosition.position = navMesh.TriangleSet[pointCoordinates.x][pointCoordinates.y].center;
			//check from position to player->position. Check all world elements and if any first contact points have a distance from the position that is less than the player's, the position is defensive, you can break and that's that.
		}
	}

}
