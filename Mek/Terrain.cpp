#include "Terrain.h"
#include "Program.h"
#include "lib\glm\gtc\matrix_transform.hpp"

#include "RayVsTriangle.h"

struct tri
{
	tri(unsigned int i0, unsigned int i1, unsigned int i2, std::vector<glm::vec3>* h)
	{
		i[0] = i0;
		i[1] = i1;
		i[2] = i2;
		v[0] = h->at(i0);
		v[1] = h->at(i1);
		v[2] = h->at(i2);
		fn = glm::cross(v[0] - v[1], v[1] - v[2]);
	}
	unsigned int i[3];
	glm::vec3 v[3];
	glm::vec3 fn;
};

Terrain::Terrain()
{
	//Ordered from 0 to n, where 0 is the flatest texture and n is the steepest
	_textures.push_back(new Texture("terrainG0.png", GL_LINEAR, GL_REPEAT));
	_textures.push_back(new Texture("terrainG1.png", GL_LINEAR, GL_REPEAT));
	_textures.push_back(new Texture("terrainG2.png", GL_LINEAR, GL_REPEAT));
	_textures.push_back(new Texture("terrainG3.png", GL_LINEAR, GL_REPEAT));
}

void Terrain::InitRender()
{
	Program::getInstance().createShader("terrain", GL_FRAGMENT_SHADER, "shaders/terrain.frag");
	Program::getInstance().createShader("terrain", GL_VERTEX_SHADER, "shaders/terrain.vert");
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	_vbo = new GLuint[3];
	glGenBuffers(3, &_vbo[0]);
	//Triangle method
	//Think quads
	std::vector<tri> triangle;

	//indexing method : arr[x + width * (y + depth * z)]
	//arr[x + _width * (y + {0,1})]
	glm::vec3* norms = new glm::vec3[2 * _width * _height];
	for (int i = 0; i < _height - 1; i++)//row
	{
		for (int j = 0; j < _width - 1; j++)//column
		{
			unsigned int rc = i * _width + j;
			unsigned int a = rc;
			unsigned int b = rc + _width;
			unsigned int c = b + 1;
			unsigned int d = a + 1;

			_indices.push_back(a);
			_indices.push_back(b);
			_indices.push_back(c);
			triangle.push_back(tri(a, b, c, &_heightMap));

			norms[i + _width * (j + 0)] = triangle.back().fn;

			_indices.push_back(c);
			_indices.push_back(a);
			_indices.push_back(d);
			triangle.push_back(tri(c, a, d, &_heightMap));

			norms[i + _width * (j + 1)] = triangle.back().fn;
		}
	}
	//Per vertex normal calculation
	for (int i = 0; i < _height - 1; i++)//row
	{
		for (int j = 0; j < _width - 1; j++)//column
		{
			glm::vec3 finalNormal;
			// Look for upper-left triangles
			if (j != 0 && i != 0)
			{
				for (int k = 0; k < 2; k++)
					finalNormal += norms[(i - 1) + _width * ((j - 1) + k)];
			}
			// Look for upper-right triangles
			if (i != 0 && j != _width - 1)
			{
				finalNormal += norms[(i - 1) + _width * (j + 0)];
			}
			// Look for bottom-right triangles
			if (j != _width - 1 && i != _height - 1)
			{
				for (int k = 0; k < 2; k++)
					finalNormal += norms[i + _width * (j + k)];
			}
			// Look for bottom-left triangles
			if (j != 0 && i != _height - 1)
			{
				finalNormal += norms[i + _width * ((j - 1) + 1)];
			}

			_normals.push_back(glm::normalize(finalNormal));
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_heightMap[0]) * _heightMap.size(), &_heightMap[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_normals[0]) * _normals.size(), &_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Terrain::RenderShadowPass()
{
	Program::getInstance().use("terrainShadow");
	glBindVertexArray(_vao);

	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProj = glm::ortho<float>(-128, 128, -128, 128, -32, 32);
	glm::mat4 depthView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 depthMVP = depthProj * depthView;
	Program::getInstance().setUniform("depthMVP", depthMVP);

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Terrain::Render(int shadowMapID)
{
	Program::getInstance().use("terrain");
	Program::getInstance().setUniform("gWVP", Camera::getInstance().matrix());
	Program::getInstance().setUniform("_min", _min);
	Program::getInstance().setUniform("_max", _max);
	//Program::getInstance().setUniform("u", u);
	//Program::getInstance().setUniform("d", d);
	glBindVertexArray(_vao);
	
	// Compute the MVP matrix from the light's point of view
	glm::mat4 depthProj = glm::ortho<float>(-32, 32, -32, 32, -8, 8);
	glm::mat4 depthView = glm::lookAt(glm::vec3(0.0f, 1.0f, 1.0f), glm::vec3(0), glm::vec3(0, 1, 0));
	glm::mat4 depthMVP = depthProj * depthView;
	glm::mat4 biasMatrix(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
		);
	depthMVP = biasMatrix*depthMVP;
	Program::getInstance().setUniform("depthMVP", depthMVP);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, _textures[0]->object());
	Program::getInstance().setUniform("terrainG0", 0);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, _textures[1]->object());
	Program::getInstance().setUniform("terrainG1", 1);
	glActiveTexture(GL_TEXTURE0 + 2);
	glBindTexture(GL_TEXTURE_2D, _textures[2]->object());
	Program::getInstance().setUniform("terrainG2", 2);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_2D, _textures[3]->object());
	Program::getInstance().setUniform("terrainG3", 3);
	glActiveTexture(GL_TEXTURE0 + 4);
	glBindTexture(GL_TEXTURE_2D, shadowMapID);
	Program::getInstance().setUniform("shadowMap", 4);

	glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Terrain::IndexAt(int x, int y)
{
	int i = ((y + _height/2) * _width + x + _width/2);
	return (i >= 0 && i < _size ? i : 0);
}

void Terrain::PosAtIndex(unsigned int* x, unsigned int* y, unsigned int index)
{
	*x = index % _width;
	*y = index / _height;
}

void Terrain::LoadHeightMap(char* fp, float heightmod, unsigned int smoothiter, float factor)
{
	unsigned int il;
	ilGenImages(1, &il);
	ilBindImage(il);
	ilLoadImage(fp);

	_height = ilGetInteger(IL_IMAGE_HEIGHT);
	_width = ilGetInteger(IL_IMAGE_WIDTH);
	unsigned int c = ilGetInteger(IL_IMAGE_CHANNELS);
	ILubyte* pixels = ilGetData();

	_size = _height * _width;
	int hw = _width / 2;
	int hh = _height / 2;
	_min = _max = 0;
	for (int y = 0; y < _width; y++)
	{
		for (int x = 0; x < _height; x++)
		{
			float h = (float)pixels[(y * _width + x) * c] * heightmod;
			_heightMap.push_back(glm::vec3(x - hw, h, y - hh));
			h > _max ? _max = h : (h < _min ? _min = h : h);
		}
	}
	ilBindImage(0);
	ilDeleteImage(il);

	for (int i = 0; i < smoothiter; i++)
		PreSmooth(factor);
}

void Terrain::GenerateHeightMap(int mode, float* heightmap, float heightmod)
{
	//TODO: Implement generation functions
}

float Terrain::LerpHeights(int a, int b, float t)
{
	a < 0 || a >= _size ? a = 0 : a;
	b < 0 || b >= _size ? b = 0 : b;
	return _heightMap[a].y * (1 - t) + _heightMap[b].y * t;
}

//This function must be run before the initRender function
void Terrain::PreSmooth(float factor)
{
	for (int i = 0; i < _height; i++)//check left pixel
		for (int j = 0; j < _width; j++)
			_heightMap[i * _width + j].y = 
				LerpHeights(i * _width + j, i * _width + (j-1), factor);
	for (int i = 0; i < _height; i++)//check up pixel
		for (int j = 0; j < _width; j++)
			_heightMap[i * _width + j].y = 
				LerpHeights(i * _width + j, (i-1) * _width + j, factor);
	for (int i = 0; i < _height; i++)//check right pixel
		for (int j = 0; j < _width; j++)
			_heightMap[i * _width + j].y = 
				LerpHeights(i * _width + j, i * _width + (j+1), factor);
	for (int i = 0; i < _height; i++)//check down pixel
		for (int j = 0; j < _width; j++)
			_heightMap[i * _width + j].y = 
				LerpHeights(i * _width + j, (i+1) * _width + j, factor);
}

float Terrain::HeightAtLocation(glm::vec3 p)
{
	glm::vec3 verts[3];
	glm::vec3 uP, dP;

	uP.x = ceil(p.x);
	uP.z = floor(p.z);

	dP.x = floor(p.x);
	dP.z = ceil(p.z);

	if (uP.x == dP.x)
		dP.x--;
	if (uP.z == dP.z)
		dP.z++;

	u = uP;
	d = dP;

	//top right
	verts[0] = _heightMap[IndexAt(uP.x, uP.z)];
	verts[1] = _heightMap[IndexAt(uP.x - 1, uP.z)];
	verts[2] = _heightMap[IndexAt(uP.x, uP.z + 1)];
	
	rvtResponse rvt;

	if (RayVsTriangle(glm::vec3(0, -1, 0), glm::vec3(p.x, 1024, p.z), verts[0], verts[1], verts[2], &rvt))
		return rvt.intersection.y;
	//not over top right, lets check bottom left
	verts[0] = _heightMap[IndexAt(dP.x, dP.z)];
	verts[1] = _heightMap[IndexAt(dP.x + 1, dP.z)];
	verts[2] = _heightMap[IndexAt(dP.x, dP.z - 1)];

	if (RayVsTriangle(glm::vec3(0, -1, 0), glm::vec3(p.x, 1024, p.z), verts[0], verts[1], verts[2], &rvt))
		return rvt.intersection.y;

	printf("Height failed! : %f, %f\n", p.x, p.z);
	return p.y - 0.5;
}