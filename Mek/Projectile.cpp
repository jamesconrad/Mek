#include "Projectile.h"


aiScene* scene;

void prepProjectiles()
{
	Model* cg = new Model();
	cg->loadModel("models/SmallCube.dae");
	scene = (aiScene*)cg->getScene();
}

Projectile::Projectile(glm::vec3 p, glm::vec3 d, float v, float _d, float lifespan,Sound* _sound)
{
	pos = p;
	dir = d;
	vel = v;
	dmg = _d;
	life = lifespan;
	go = new GameObject(0);
	cc = new ComponentCollision();
	cg = new Model();
	go->SetName("Projectile");
	go->AddComponent(GRAPHICS, cg);
	go->AddComponent(PHYSICS, cc);
	cg->setOwner(go);
	cc->setOwner(go);
	cg->setFilepath("models/SmallCube.dae");
	cg->loadScene(scene);
	cc->setCollisionMask(scene);
	cc->type = PROJ;
	go->pos = p;
	go->dir = d;
	go->scale = glm::vec3(0.1, 0.1, 0.1); 
	go->dmg = _d;
	go->SetName("PlayerProjectile");
	go->health = 100.f;



	sound = _sound;
	ChannelPtr = sound->Play();

	FMOD_VECTOR pos = { p.x, p.y, p.z };
	FMOD_VECTOR vel = { d.x, d.y, d.z };
	isSound = true;

	cc->createHitboxRender();

	go->handle = ObjectManager::instance().pMap.size();

	alive = true;
}