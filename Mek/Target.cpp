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