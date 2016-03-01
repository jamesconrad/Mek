#include "NavMesh.h"

inline void NavMesh::loadNavMesh(char* filename)
{
	FILE *fileStream = fopen(filename, "r");
	char current_line[128];

	if (fileStream == nullptr)
	{
		throw std::runtime_error("Navmesh file failed to load. Recommended check: The file path");
	}

	while (fgets(current_line, 128, fileStream))
	{
		if (current_line[0] == '/' || current_line[0] == '#' || current_line[0] == '\n')
		{
			continue;
		}

		if (current_line[0] == 'v' && current_line[1] == ' ')
		{
			add_vertices(current_line);
		}

		if (current_line[0] == 'f')
		{
			add_indices(current_line);
		}
	}
	

	for (unsigned int i = 0; i < indices.size(); i++)
		triangles.push_back(NavFace(vertices[indices[i].vertexCoords[0]-1].vertexCoords, vertices[indices[i].vertexCoords[1]-1].vertexCoords, vertices[indices[i].vertexCoords[2]-1].vertexCoords));

	sortTrianglesInto2DVector();
}

inline void NavMesh::add_vertices(char* line)
{
	strtok(line, " \t\n\r");

	float vertX, vertY, vertZ;
	vertX = atof(strtok(NULL, " \t\n\r"));
	vertY = atof(strtok(NULL, " \t\n\r"));
	vertZ = atof(strtok(NULL, " \t\n\r"));

	vertices.push_back(NavVertex(vertX, vertY, vertZ));
}

inline void NavMesh::add_indices(char *line)
{
	unsigned int vert1;
	unsigned int vert2;
	unsigned int vert3;

	std::vector<char *>	sets;
	char *currentToken;

	currentToken = strtok(line, " \t\n\r");
	currentToken = strtok(NULL, " \t\n\r");
	while (currentToken != NULL)
	{
		sets.push_back(currentToken);
		currentToken = strtok(NULL, " \t\n\r");

	}

	for (unsigned int i = 0; i < sets.size(); i+=3)
	{
		vert1 = (atoi(strtok(sets[i], " /")));
		vert2 = (atoi(strtok(sets[i+1], " /")));
		vert3 = (atoi(strtok(sets[i+2], " /")));

		indices.push_back(NavVertex(vert1, vert2, vert3));
	}
}

inline void NavMesh::sortTrianglesInto2DVector()
{
	std::vector<NavFace> oneWayVector;
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		for (unsigned int j = i; j > 0 && triangles[j].center.z < triangles[j - 1].center.z; j--)
		{
			NavFace temp = triangles[j];
			triangles[j] = triangles[j - 1];
			triangles[j - 1] = temp;
		}
	}
	NavFace temp = triangles[0];
	std::vector<NavFace> tempVector;
	for (unsigned int i = 0; i < triangles.size(); i++)
	{
		if (triangles[i].center.z == temp.center.z)
		{
			tempVector.push_back(triangles[i]);
		}
		else
		{
			TriangleSet.push_back(tempVector);
			tempVector.clear();
			tempVector.shrink_to_fit();

			temp = triangles[i];
			tempVector.push_back(triangles[i]);
		}
	}
	TriangleSet.push_back(tempVector);

	for (unsigned int i = 0; i < TriangleSet.size(); i++)
	{
		for (unsigned int j = 0; j < TriangleSet[i].size(); j++)
		{
			for (unsigned int k = j; k > 0 && TriangleSet[i][k].center.x < TriangleSet[i][k - 1].center.x; k--)
			{
				NavFace temp = TriangleSet[i][k];
				TriangleSet[i][k] = TriangleSet[i][k - 1];
				TriangleSet[i][k - 1] = temp;
			}
		}
	}
}

inline glm::vec2 NavMesh::isPointInsideTriangle(glm::vec3 &_position)
{
	for (unsigned int i = 0; i < TriangleSet.size(); i++)
	{
		for (unsigned int j = 0; j < TriangleSet[i].size(); j++)
		{
			if (RayVsTriangle(glm::vec3(0, -1, 0), _position, TriangleSet[i][j].vertexA.vertexCoords, TriangleSet[i][j].vertexB.vertexCoords, TriangleSet[i][j].vertexC.vertexCoords, &rayVsTri))
			{
				return glm::vec2(i, j);
			}
		}
	}
	return glm::vec2(-1, -1);
}

inline glm::vec2 NavMesh::isTriangleInsideTriangle(NavFace &face)
{
	for (unsigned int i = 0; i < TriangleSet.size(); i++)
	{
		for (unsigned int j = 0; j < TriangleSet[i].size(); j++)
		{
			if (face.center == TriangleSet[i][j].center)
				return glm::vec2(i, j);
		}
	}
}