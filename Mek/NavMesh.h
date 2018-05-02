//Jordan Culver - February 2016

#ifndef NAVMESH_H
#define NAVMESH_H

#include <vector>
#include "RayVsTriangle.h"
#include "lib\glm\glm.hpp"
#include "lib\glm\vec3.hpp"

#ifndef NAVVERTEX
#define NAVVERTEX
struct NavVertex
{
	NavVertex()
	{
		vertexCoords = glm::vec3(0.0f);
	}

	NavVertex(glm::vec3 _vertexCoords)
	{
		vertexCoords = _vertexCoords;
	}

	NavVertex(float x, float y, float z)
	{
		vertexCoords.x = x; vertexCoords.y = y; vertexCoords.z = z;
	}

	glm::vec3 vertexCoords;


};
#endif

#ifndef NAVFACE
#define NAVFACE
struct NavFace
{
	NavFace()
	{

	}
	NavFace(glm::vec3 vertA, glm::vec3 vertB, glm::vec3 vertC)
	{
		vertexA = vertA;
		vertexB = vertB;
		vertexC = vertC;

		center = glm::vec3(vertA + vertB + vertC);
		center = glm::vec3(center.x / 3, center.y / 3, center.z / 3);
	}
	NavVertex vertexA;
	NavVertex vertexB;
	NavVertex vertexC;

	glm::vec3 center;

	NavFace *parentPoint = nullptr;

	float f_score;
	float g_score;
	float h_score;

	int xStart; int yStart;
};
#endif // !NAVVACE


class NavMesh
{
public:
	NavMesh()
	{

	}
	inline void loadNavMesh(char* filename);
	NavMesh(char* filename);
	~NavMesh()
	{}

	inline void evaluatePoints();

	//returns the index of the intersecting triangle in the triangles vector. Returns -1 if the position is not inside any of the triangles;
	inline glm::vec2 isPointInsideTriangle(glm::vec3 &position);
	inline glm::vec2 isTriangleInsideTriangle(NavFace &face);

	static NavMesh& getInstance()
	{
		static NavMesh instance;
		return instance;
	}

private:
	inline void add_vertices(char *line);
	inline void add_indices(char *line);

	inline void sortTrianglesInto2DVector();

public:
	std::vector<NavVertex> vertices;
	std::vector<NavVertex> indices;
	std::vector<NavFace> triangles;

	std::vector<NavVertex> closedSet;
	std::vector<NavVertex> openSet;

	std::vector<std::vector<NavFace>> TriangleSet;

	float g_score, h_score, f_score;
	float tentativeG_score;

	rvtResponse rayVsTri;
};

#include "NavMesh.inl"
#endif