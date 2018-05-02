// Jordan Culver
// Febrary 2016

#pragma once

#include <utility>

#include "lib\glm\glm.hpp"

inline bool RayVsPlane(float &origin, float &dir, float axisMin, float axisMax, float &tFirst, float &tLast)
{
	if (fabs(dir) < 0.000001) // if the direction is non existant (less than near zero), check if the origin is inside the plane
	{
		return (origin <= axisMax && origin >= axisMin);
	}

	float tMin = (axisMin - origin) / dir;
	float tMax = (axisMax - origin) / dir;
	if (tMin > tMax) std::swap(tMin, tMax); // Make sure that tMin and tMax are the min and max relative to the position of the origin, rather than the axis maxs and mins.

	if (tMax < tFirst || tMin > tLast) // If it is outside the 1d plane, return false;
		return false;

	if (tMin > tFirst) tFirst = tMin; //Not sure which these are doing yet. Gimme a minute.
	if (tMax < tLast) tLast = tMax;

		return true;
}

inline bool RayVsOBB(glm::vec3 origin, glm::vec3 &Direction, glm::vec3 minVert, glm::vec3 maxVert)
{
	float tFirst = 0.0f;
	float tLast = LONG_MAX;
	float t;

	if (!RayVsPlane(origin.x, Direction.x, minVert.x, maxVert.x, tFirst, tLast)) return false;
	if (!RayVsPlane(origin.y, Direction.y, minVert.y, maxVert.y, tFirst, tLast)) return false;
	if (!RayVsPlane(origin.z, Direction.z, minVert.z, maxVert.z, tFirst, tLast)) return false;

	t = tFirst;
	// I'm not sure what t is yet. I'll figure it out eventually.
	return true;
}