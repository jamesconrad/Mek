#pragma once

#include <vector>
#include "lib\glm\glm.hpp"

enum lerp_mode { LINEAR = 1, BEZIER2 = 2, CATMULLROM = 3, SLERP = 4 };

struct Interpolation
{
	// Points to lerp around
	std::vector<glm::vec3> points;

	// pos Position
	glm::vec3 pos;

	// Some Vars
	lerp_mode state = SLERP;
	int stage = 0;
	int cycles = 0;
	float time = 0.0f;
	bool curveFin = true;
	bool paused = false;
	bool loop = true;

	void interpolate(float dTime);


};