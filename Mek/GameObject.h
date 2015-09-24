#pragma once

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
private:
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

private:
	int _handle;
	std::string _objStr;
	std::unordered_multimap<ComponentId, Component*> _components;
};