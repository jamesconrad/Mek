#include "AIBehaviour.h"
#include <iostream>


//Returns -1 if object is not found. Else, returns the index of it's location.
inline int AiBehaviour::isObjectInVector(std::vector<NavFace*> &vector, NavFace &node)
{
	for (unsigned int k = 0; k < vector.size(); k++)
	{
		if (vector[k]->center == node.center)
			return k;
	}
	return -1;
}

inline glm::vec3 AiBehaviour::update(glm::vec3 &_position, glm::vec3 &_heading, float &dT)
{
	glm::vec3 wanderTarget = glm::vec3(randomClampedNumber() * wanderJitter, 0.0f, randomClampedNumber() * wanderJitter);
	wanderTarget = glm::normalize(wanderTarget);
	wanderTarget *= wanderRadius;

	wanderTarget += _position + _heading * wanderDistance;

	heading = glm::normalize(wanderTarget - _position);
	return heading;
}

inline void AiBehaviour::determineRoute(NavMesh &mesh, NavFace &start, NavFace &goal)
{
	glm::vec2 startIndex = mesh.isTriangleInsideTriangle(start);
	glm::vec2 endIndex = mesh.isTriangleInsideTriangle(goal);

	if (closedSet.size() != 0)
	{
		closedSet.clear();
		closedSet.shrink_to_fit();
	}
	/*while (!openSet.empty())
	{
		openSet.pop();
	}*/
	if (openSet2.size() != 0)
	{
		openSet2.clear();
		openSet2.shrink_to_fit();
	}
	start.parentPoint = nullptr;
	//openSet.push(&start);
	openSet2.push_back(&start);
	NavFace *current = &start;
	current->xStart = startIndex.x;
	current->yStart = startIndex.y;
	NavFace *neighbor = &start;
	NavFace *finalNode = nullptr;
	glm::vec3 tentative_Point;
	float tentative_g_current;
	float tentative_h_current;
	float tentative_f_cost;
	int neighborIndexInOpenSet = -1;

	if (start.center == goal.center)
	{
		path.clear();
		path.push_back(&start);
		return;
	}

	start.f_score = 0;
	start.g_score = 0;
	start.h_score = glm::length(goal.center - start.center);

	while (!openSet2.empty())
	{
		for (unsigned int i = 0; i < openSet2.size(); i++)
		{
			current = openSet2[0];
			if (openSet2[i]->f_score < current->f_score)
				current = openSet2[i];
		}
		neighborIndexInOpenSet = isObjectInVector(openSet2, *current);
		if (neighborIndexInOpenSet != -1)
			openSet2.erase(openSet2.begin() + neighborIndexInOpenSet);	
				
		for (int i = -1; i <= 1; i++)
		{
			if (current->xStart + i == -1) //checks for out of bounds x coord
				continue;
			else if (current->xStart + i >= mesh.TriangleSet.size() - 1)
				continue;
			
			for (int j = -1; j <= 1; j++)
			{
				if (current->yStart + j == -1) //checks for out of bounds y coord
					continue;
				else if (current->yStart + j >= mesh.TriangleSet[i + current->xStart].size() - 1)
					continue;
				if (i == 0 && j == 0)
					continue;
				
				if (mesh.TriangleSet[i + current->xStart][j + current->yStart].center == goal.center)
				{
					finalNode = &goal;
					finalNode->parentPoint = current;
					finalNode->g_score = current->g_score;
					finalNode->h_score = 0;
					finalNode->f_score = current->g_score;
					finalNode->xStart = current->xStart + i;
					finalNode->yStart = current->yStart + j;

					path.clear();
					path.push_back(finalNode);
					NavFace* tempParent = path.back()->parentPoint;
					while (tempParent != nullptr)
					{
						path.push_back(tempParent);
						tempParent = tempParent->parentPoint;
					}
					return;
				}
				neighbor = &mesh.TriangleSet[i + current->xStart][j + current->yStart];
				neighborIndexInOpenSet = isObjectInVector(openSet2, *neighbor);
				if (neighborIndexInOpenSet != -1)
				{
					if (openSet2[neighborIndexInOpenSet]->f_score <= neighbor->f_score)
						continue;
				}
				neighborIndexInOpenSet = isObjectInVector(closedSet, *neighbor);
				if (neighborIndexInOpenSet != -1)
				{
					if (closedSet[neighborIndexInOpenSet]->f_score <= neighbor->f_score)
						continue;
				}
				neighbor->parentPoint = current;
				neighbor->g_score = neighbor->parentPoint->g_score + glm::length(neighbor->center - neighbor->parentPoint->center);
				neighbor->h_score = fabs(glm::distance(neighbor->center, goal.center));
				neighbor->f_score = neighbor->g_score + neighbor->h_score;
				neighbor->xStart = current->xStart + i;
				neighbor->yStart = current->yStart + j;
				openSet2.push_back(neighbor);
			}
			
		}
		closedSet.push_back(current);
	}
}