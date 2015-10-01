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
	glm::vec3 GetPos();
	glm::vec3 GetScale();
	glm::vec3 GetRot();

private:
	glm::vec3 pos, scale, rot;
	int _handle;
	std::string _objStr;
	std::unordered_multimap<ComponentId, Component*> _components;
};