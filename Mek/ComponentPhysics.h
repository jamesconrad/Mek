#pragma once

#include "Component.h"

class ComponentPhysics : public Component
{
	std::pair<float, float> direction;

	float speed;
	float acceleration;
};
