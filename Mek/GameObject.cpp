#include "GameObject.h"
#include "Projectile.h"

GameObject::GameObject(int h)
{
	handle = ObjectManager::instance().addObject(this);
	_n = "NOT_SET";
	pos = glm::vec3(0, 0, 0);
	scale = glm::vec3(1, 1, 1);
	rot = glm::vec3(0, 0, 0);
	vel = 0;
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

void ObjectManager::addProjectile(Projectile* p)
{
	p->alive = true;
	p->handle = pMap.size();
	p->go->handle = p->handle;
	pMap.push_back(p);
}

void ObjectManager::removeProjectile(int index)
{
	for (int i = 0, s = pMap.size(); i < s; i++)
	{
		if (pMap[i]->alive == false)
		{
			free(pMap[i]);
			pMap.erase(pMap.begin() + index);
		}
	}
}