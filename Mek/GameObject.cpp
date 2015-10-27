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
	handle = ObjectManager::instance().addObject(this);
	_n = "NOT_SET";
	pos = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rot = glm::vec3(0, 0, 0);
	vel = glm::vec3(0, 0, 0);
}

void GameObject::AddComponent(ComponentId ctype, Component *comp)
{
	if (_components.find(ctype) != _components.end())
	{
		printf("WARN: Component type %i already exists for Object handle %i\nAdding component anyways, this may cause unintended results.", ctype, handle);
	}
	
	ObjectManager::instance().addToMap(ctype, handle);

	comp->setOwner(this);
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

glm::vec3 Component::getPos() 
{ 
	return _owner->pos;
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

int ObjectManager::addObject(GameObject* o) 
{
	gMap.push_back(o);
	return gMap.size() - 1;
}

void ObjectManager::addComponent(ComponentId ctype, Component* c, int handle)
{
	gMap[handle]->AddComponent(ctype, c);
}

void ObjectManager::addToMap(ComponentId ctype, int handle)
{
	if (ctype == PHYSICS)
		colMap.push_back(handle);
}