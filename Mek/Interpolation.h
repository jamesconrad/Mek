#pragma once

#include <vector>
#include "lib\glm\glm.hpp"

enum lerp_mode { LINEAR = 1, BEZIER2 = 2, CATMULLROM = 3, SLERP = 4 };

template <typename T>
T lerp(T d0, T d1, float t)
{
	return d0 * (1 - t) + d1 * t;
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

	void interpolate(float dTime);
	void speedControlInterp(float dTime);
};