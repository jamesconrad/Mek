#pragma once

#include "Component.h"
#include <unordered_map>

enum ComponentType
{
	Graphics,
	LightSource,
	Physics
};


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
	std::unordered_multimap<ComponentType, Component*> _components;
};

// overall component notes
/*

Shader and Camera become Singletons

ComponentGraphics hooks into Shader to render without recompiling shaders for every object

Shader requires an update every frame to resync the Camera matrix

Shader contains a std::map<std::string, int> where the string refers to the shader name, and the int refers to program
-ComponentGraphics asks for the program variable by the string

*/


// to check inside the map:
/*

 v--is the variable name assoicaited with the enum passed
auto its = map.equal_range(LightSource);
for (auto it = its.first; it != its.second; ++it)
{
// it needs to be cast into a Light component in this example
it->first = ComponentType
it->second = Component*
}

*/