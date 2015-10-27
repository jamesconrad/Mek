#pragma once

#include "lib\glm\common.hpp"
#include "Component.h"
#include <unordered_map>

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

	glm::vec3 pos, scale, rot, vel;
	int handle;
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

	//singleton
	static ObjectManager& instance()
	{
		static ObjectManager instance;
		return instance;
	}

	std::vector<GameObject*> gMap;
	std::vector<unsigned int> colMap;
private:
	ObjectManager() {}
};