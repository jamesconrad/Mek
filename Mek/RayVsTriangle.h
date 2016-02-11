#pragma once
#include "lib\glm\common.hpp"
#include "lib\glm\geometric.hpp"
#include "lib\glm\gtx\projection.hpp"

struct rvtResponse
{
	glm::vec3 bintersection;
	glm::vec3 intersection;
	glm::vec2 barycoords;
	float distance;
};

bool RayVsTriangle(glm::vec3 &ray, glm::vec3 &origin,
	glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2,
	rvtResponse* out, bool cullbackface = false);