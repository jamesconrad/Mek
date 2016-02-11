#pragma once

#include <vector>
#include "lib\glm\glm.hpp"

enum lerp_mode { LINEAR = 1, BEZIER2 = 2, CATMULLROM = 3, SLERP = 4 };

template <typename T>
T lerp(T d0, T d1, float t)
{
	return d0 * (1 - t) + d1 * t;
}
template <typename T>
T slerp(T p0, T p1, float deltaT)
{
	float theta = acosf(glm::dot(p0, p1));
	if (theta == 0)
		return p0;
	else if (theta == 180)
		return lerp(p0, p1, deltaT);
	else
		return p0 * ((sin((1 - deltaT) * theta)) / sin(theta)) + p1 * ((sin(deltaT * theta)) / sin(theta));
}

template <typename T>
float invlerp(T d0, T d1, T res)
{
	return (res + (d0*-1)) / (d1 + (d0*-1));
}

struct ce
{
	glm::vec3 v;
	float tval;
	float arcl;
	int seg;
};

struct Interpolation
{
	// Points to lerp around
	std::vector<glm::vec3> points;
	std::vector<ce> curve;

	float arclength(int i);
	void buildCurve();

	// pos Position
	glm::vec3 pos;

	// Some Vars
	lerp_mode state = SLERP;
	int stage = 0;
	int cycles = 0;
	float time = 0.0f;
	bool curveFin = false;
	bool paused = false;
	bool loop = true;

	bool isFinished = false;
	glm::vec3 finalCheck;

	void interpolate(float dTime);
	void speedControlInterp(float dTime);
};