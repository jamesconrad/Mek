#pragma once

#include "lib\glm\common.hpp"
#include "Texture.h"
#include "include\GL\glew.h"
#include <vector>



class Terrain
{
public:
	Terrain();

	void LoadHeightMap(char* fp, float heightmod, unsigned int smoothiter = 0, float factor = 0.f);
	void GenerateHeightMap(int mode, float* heightmap, float heightmod);

	void InitRender();
	void Render(int shadowMapID);
	void RenderShadowPass();
	float HeightAtLocation(glm::vec3 pos);
private:
	unsigned int IndexAt(int x, int y);
	void PosAtIndex(unsigned int* x, unsigned int* y, unsigned int index);

	void PreSmooth(float factor);
	float LerpHeights(int a, int b, float t);

	GLuint _vao;
	GLuint* _vbo;

	std::vector<Texture*> _textures;
	std::vector<GLuint> _indices;
	std::vector<glm::vec3> _normals;
	std::vector<glm::vec3> _heightMap;//Vec3 for simplyfying rendering process, may need to change this later
	unsigned int _height, _width, _size;
	float _min, _max;
	glm::vec3 u, d;
};