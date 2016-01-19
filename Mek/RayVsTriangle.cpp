#include "RayVsTriangle.h"

//Using the Moller Trumbore algorithm
bool RayVsTriangle(glm::vec3 ray, glm::vec3 origin, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, rvtResponse* out)
{
	glm::vec3 edge0 = p1 - p0;
	glm::vec3 edge1 = p2 - p0;
	glm::vec3 P = glm::cross(ray, edge1);
	float det = glm::dot(edge0, P);

	if (det < NEAR_ZERO)	//If det is negative the triangle is backfacing and should be ignored
		return false;		//If det is near zero the ray and triangle are parallel

	float invdet = 1 / det;

	glm::vec3 T = origin - p0;
	out->barycoords.x = glm::dot(T, P) * invdet;
	if (out->barycoords.x < 0 || out->barycoords.x > 1)	//Point is outside triangle
		return false;

	glm::vec3 Q = glm::cross(T, edge0);
	out->barycoords.y = glm::dot(ray, Q) * invdet;
	if (out->barycoords.y < 0 || out->barycoords.y > 1)	//Point is outside triangle
		return false;

	//Now we have a collision
	//Convert from barycentric
	out->intersection = glm::vec3(out->barycoords.x * p0,
		out->barycoords.y * p1,
		(1 - out->barycoords.x - out->barycoords.y) * p2);
	//And calculate distance
	out->distance = glm::dot(edge1, Q) * invdet;
	return true;
}