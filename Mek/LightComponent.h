#pragma once

#include "Component.h"
#include "lib\glm\glm.hpp"

#include "Program.h"


enum LightType
{
	SPOT,
	AMBIENT,
	DIFFUSE,
	SPECCULAR
};

class LightComponent : public Component
{
public:
	LightType _type;
	glm::vec3 _pos;
	glm::vec3 _dir;
	glm::vec3 _col;
	float _attenuation;
	float _ambientCoefficient;
	float _coneAngle;

	LightComponent(LightType l) { _type = l; _handle = Program::getInstance().addLightSource(this); }
	~LightComponent() { Program::getInstance().delLightSource(_handle); }

	void SetVars(LightType l, glm::vec3 pos, glm::vec3 dir, glm::vec3 color, float attenuation, float ambient, float coneAngle)
	{
		_type = l;
		_pos = pos;
		_dir = dir;
		_col = color;
		_attenuation = attenuation;
		_ambientCoefficient = ambient;
		_coneAngle = coneAngle;
	}
private:
	int _handle;
};