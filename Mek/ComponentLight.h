#pragma once

#include "Component.h"
#include "lib\glm\glm.hpp"

#include "Program.h"


enum LightType
{
	lSPOT,
	lPOINT,
	lDIR
};
//the actual lights
struct BaseLight
{
	glm::vec3 Color;
	float AmbientIntensity;
	float DiffuseIntensity;
};

struct DirectionalLight
{
	BaseLight Base;
	glm::vec3 Direction;
};

struct Attenuation
{
	float Constant;
	float Linear;
	float Exp;
};

struct PointLight
{
	BaseLight Base;
	glm::vec3 Position;
	Attenuation Atten;
};

struct SpotLight
{
	PointLight Base;
	glm::vec3 Direction;
	float Cutoff;
};

class LightComponent : public Component
{
public:
	//void pointer is used to allow for two different variables to use the same pointer
	//downside is using a static cast to convert
	//to get the proper object:
	//light_ptr = static_cast<TYPE*>(lightSource);
	LightType type;
	void* lightSource;
	LightComponent(LightType l)
	{
		type = l;
		_handle = Program::getInstance().addLightSource(this);
		if (type == lSPOT)
		{
			lightSource = new SpotLight;
		}
		else if (type == lPOINT)
		{
			lightSource = new PointLight;
		}
	}
	~LightComponent() { Program::getInstance().delLightSource(_handle); }

	void SetVars(LightType l, void* data)
	{
		type = l;
		free(lightSource);
		if (type == lSPOT)
		{
			lightSource = new SpotLight;
		}
		else if (type == lPOINT)
		{
			lightSource = new PointLight;
		}
		lightSource = data;
	}

	void update() {}
private:
	int _handle;
};

/*
uniform int gNumPointLights;
uniform int gNumSpotLights;

uniform DirectionalLight gDirectionalLight;
uniform PointLight gPointLights[MAX_POINT_LIGHTS]; //MAX_POINT_LIGHTS = 16;
uniform SpotLight gSpotLights[MAX_SPOT_LIGHTS]; //MAX_SPOT_LIGHTS = 16;

//ALREADY DONE uniform sampler2D gColorMap;
//ALREADY DONE uniform vec3 gEyeWorldPos;

uniform float gMatSpecularIntensity;
uniform float gSpecularPower;
*/