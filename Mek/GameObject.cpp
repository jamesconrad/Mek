#include "GameObject.h"
/*GameObject(int handle);
void AddComponent(Component *comp);
bool HasComponent(ComponentId);
Component* GetComponent(ComponentId);*/
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

GameObject::GameObject(int h)
{
	_handle = h;
	//pos = glm::vec3(0, 0, 0);
	//scale = glm::vec3(0, 0, 0);
	//rot = glm::vec3(0, 0, 0);
}

void GameObject::AddComponent(ComponentId ctype, Component *comp)
{
	if (_components.find(ctype) != _components.end())
	{
		printf("WARN Component type %i already exists for Object handle %i\nAdding component anyways, this may cause unintended results.", ctype, _handle);

	}
	_components.emplace(ctype, comp);
}

bool GameObject::HasComponent(ComponentId ctype)
{
	if (_components.find(ctype) != _components.end())
	{
		return true;
	}
	else
		return false;
}

Component* GameObject::GetComponent(ComponentId ctype)
{
	auto clist = _components.find(ctype);
	return clist->second;
}

void GameObject::UpdateAll()
{
	for (unsigned i = 0; i < _components.bucket_count(); ++i) 
	{
		for (auto local_it = _components.begin(i); local_it != _components.end(i); ++local_it)
		{
			local_it->second->update();
		}
	}
}

glm::vec3 GameObject::GetPos()
{
	return pos;
}
glm::vec3 GameObject::GetScale()
{
	return scale;
}
glm::vec3 GameObject::GetRot()
{
	return rot;
}