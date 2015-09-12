#pragma once

#include "Component.h"
#include <vector>



class GameObject
{
public:
	GameObject(int handle);
	void AddComponent(Component *comp);
	bool HasComponent(ComponentId);
	Component* GetComponent(ComponentId);

private:
	int _handle;
	std::string _objStr;
	std::vector<Component*> _components;
};