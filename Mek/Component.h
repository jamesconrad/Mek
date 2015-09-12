#pragma once
// required to fix declaration loop
class GameObject;

enum ComponentId
{
	CONTROLLER,
	PHYSICS,
	GRAPHICS
};

class Component
{
public:
	virtual void update() = 0;
	void setOwner(GameObject* owner) { _owner = owner; }
	GameObject* getOwner() { return _owner; }
private:
	GameObject* _owner;
};