#pragma once

#include "lib\glm\common.hpp"
#include "Component.h"
#include <unordered_map>

class Projectile;

class GameObject;

enum ComponentId
{
	CONTROLLER,
	PHYSICS,
	GRAPHICS,
	LIGHT
};

class Component
{
public:
	virtual void update() = 0;
	virtual void* getBoneInfoRef() { return 0; }
	void setOwner(GameObject* owner) 
	{ 
		_owner = owner;
	}
	GameObject* getOwner() { return _owner; }
	glm::vec3 getPos();
protected:
	GameObject* _owner;
};

class GameObject
{
public:
	GameObject(int handle);
	void AddComponent(ComponentId ctype, Component *comp);
	bool HasComponent(ComponentId);
	Component* GetComponent(ComponentId);
	void UpdateAll();
	void SetName(char* name) { _n = name; }
	char* GetName() { return _n; }

	glm::vec3 pos, scale, rot, dir;
	float vel;
	glm::vec3 force = { 0.f, 0.f, 0.f };
	int handle;
	float health = 100;
	float dmg = 0;
private:
	char* _n;
	std::string _objStr;
	std::unordered_multimap<ComponentId, Component*> _components;
};

class ObjectManager
{
public:
	int addObject(GameObject*);
	void addComponent(ComponentId ctype, Component* c, int handle);
	void addToMap(ComponentId ctype, int handle);

	void addProjectile(Projectile*);
	void updateProjectile(float dTime);

	void addEnemyProjectile(Projectile*);
	void updateEnemyProjectile(float &dTime);

	//singleton
	static ObjectManager& instance()
	{
		static ObjectManager instance;
		return instance;
	}

	std::vector<GameObject*> gMap;
	std::vector<unsigned int> colMap;
	std::vector<Projectile*> enemyPMap;
	std::vector<Projectile*> pMap;

	//int currentEnemyToUpdate = 0;
private:
	ObjectManager() {}
};