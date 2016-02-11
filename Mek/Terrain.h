#pragma once

#include "lib\glm\common.hpp"
#include "Texture.h"
#include "include\GL\glew.h"
#include <vector>



class Terrain
{
public:
	Terrain();

	void LoadHeightMap(char* fp, float heightmod);
	void GenerateHeightMap(int mode, float* heightmap, float heightmod);

	void InitRender();
	void Render();

	float HeightAtLocation(glm::vec3 pos);
private:
	unsigned int IndexAt(unsigned int x, unsigned int y);
	void PosAtIndex(unsigned int* x, unsigned int* y, unsigned int index);
	GLuint _vao;
	GLuint* _vbo;

	std::vector<Texture*> _textures;
	std::vector<GLuint> _indices;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec3> _heightMap;//Vec3 for simplyfying rendering process, may need to change this later
	unsigned int _height, _width, _size;
	float _min, _max;
};