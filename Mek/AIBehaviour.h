//Jordan Culver - February 2016
#include <queue>
#include <functional>
#include "lib\glm\glm.hpp"
#include "NavMesh.h"
#include "RayVsTriangle.h"

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

		AToB = glm::normalize(vertexB.vertexCoords - vertexA.vertexCoords);
		AToC = glm::normalize(vertexC.vertexCoords - vertexA.vertexCoords);

		dot00 = glm::dot(AToC, AToC);
		dot01 = glm::dot(AToC, AToB);
		dot11 = glm::dot(AToB, AToB);
	}
	NavVertex vertexA;
	NavVertex vertexB;
	NavVertex vertexC;

	glm::vec3 AToB;
	glm::vec3 AToC;
	glm::vec3 AToP;

	float dot00;
	float dot01;
	float dot02;
	float dot11;
	float dot12;

	float u, v;
	float invDenom;

	glm::vec3 center;

	NavFace *parentPoint = nullptr;

	float f_score;
	float g_score;
	float h_score;

	int xStart; int yStart;

};
#endif // !NAVVACE


#ifndef GREATERTHANBYFSCORE
#define GREATERTHANBYFSCORE
struct GreaterThanByFScore
{
	GreaterThanByFScore()
	{}
	bool operator()(const NavFace *left, const NavFace *right) const
	{
		return left->f_score < right->f_score;
	}
};
#endif

#ifndef AIBEHAVIOUR_H
#define AIBEHAVIOUR_H

enum BehaviourState
{
	WANDERING,
	FIGHTING,
	INTERPOLATING
};

class AiBehaviour
{
public:
	AiBehaviour()
	{
		heading = { 0.0f, 0.0f, 0.0f };
		wanderDistance = 15.0f;
		wanderRadius = 1.0f;
		wanderJitter = 0.05f;

		currentBehaviour = INTERPOLATING;
	}
	~AiBehaviour()
	{}

	inline glm::vec3 update(glm::vec3 &position, glm::vec3 &heading, float &deltaTime);

	inline void determineRoute(NavMesh &mesh, NavFace &start, NavFace &goal);

	inline int isObjectInVector(std::vector<NavFace*> &vector, NavFace &node);
	//inline bool objectIsInView(glm::vec3 &position);

private:
	//inline glm::vec3 avoidObject();
	//inline glm::vec3 Wander();
	//inline glm::vec3 Fight();
private:
	glm::vec3 heading;

	float wanderDistance;
	float wanderRadius;
	float wanderJitter;


public:
	BehaviourState currentBehaviour;

	rvtResponse rayVsTri;

	std::vector<NavFace*> closedSet;

	//std::priority_queue<NavFace*, std::vector<NavFace*>, GreaterThanByFScore> openSet;
	std::vector<NavFace*> openSet2;

	std::vector<NavFace*> path;
};


//number between 1 and -1
inline float randomClampedNumber()
{
//	srand((unsigned)time(NULL));
	return (0.0f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (2.0f - 0.0f)))) - 1.0f;
}

//float between given min and max
inline float randomClampedFloat(float min, float max)
{
//	srand((unsigned)time(NULL));
	return (min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min))));
}

//Integer between given min and max
inline int randomClampedInt(int min, int max)
{
//	float returnTime = (min + static_cast <int> (rand()) / (static_cast <int> (RAND_MAX / (max - min))));
	return rand() % max - min;
}

#include "AIBehaviour.inl"
#endif