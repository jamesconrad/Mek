#include "Terrain.h"
#include "Program.h"

#include "RayVsTriangle.h"

Terrain::Terrain()
{

}

void Terrain::InitRender()
{
	Program::getInstance().createShader("terrain", GL_FRAGMENT_SHADER, "shaders/terrain.frag");
	Program::getInstance().createShader("terrain", GL_VERTEX_SHADER, "shaders/terrain.vert");
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	_vbo = new GLuint[2];
	glGenBuffers(2, &_vbo[0]);

	printf("%i,%i\n", _vbo[0], _vbo[1]);
	
	//Prep the indicies
	//Sloppy triangle strip method
	for (int srow = 0; srow < _height - 1; srow++)
	{
		for (int i = 0; i < _width; i++)
		{
			GLuint rmod = srow * _width;
			_indices.push_back(rmod + i);
			_indices.push_back(rmod + i + _width);
		}
	}
	//Triangle method
	//Think quads
	//for (int r = 0; r < _height - 1; r++)
	//{
	//	for (int c = 0; c < _width - 1; c++)
	//	{
	//		_indices.push_back(IndexAt(r,c));
	//		_indices.push_back(IndexAt(r, c + 1));
	//		_indices.push_back(IndexAt(r + 1, c));
	//
	//		_indices.push_back(IndexAt(r, c));
	//		_indices.push_back(IndexAt(r + 1, c + 1));
	//		_indices.push_back(IndexAt(r + 1, c));
	//		
	//	}
	//}

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_heightMap[0]) * _heightMap.size(), &_heightMap[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Terrain::Render()
{
	Program::getInstance().use("terrain");
	Program::getInstance().setUniform("terrain", "gWVP", Camera::getInstance().matrix());
	Program::getInstance().setUniform("terrain", "_min", _min);
	Program::getInstance().setUniform("terrain", "_max", _max);
	glBindVertexArray(_vao);
	glDrawElements(GL_TRIANGLE_STRIP, _indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int Terrain::IndexAt(unsigned int x, unsigned int y)
{
	if (x > _width || y > _height)
		return 0;
	else
		return (y * _width + x);
}

void Terrain::PosAtIndex(unsigned int* x, unsigned int* y, unsigned int index)
{
	*x = index % _width;
	*y = index / _height;
}

void Terrain::LoadHeightMap(char* fp, float heightmod)
{
	Bitmap* hm = new Bitmap();
	hm->bitmapFromFile(std::string(fp));
	_height = hm->height();
	_width = hm->width();
	_size = _height * _width;
	int hw = _width / 2;
	int hh = _height / 2;
	_min = _max = 0;
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			float h = (float)(hm->getPixel(x, y)[0]) * heightmod;
			_heightMap.push_back(glm::vec3(x - hw, h, y - hh));
			h > _max ? _max = h : (h < _min ? _min = h : h);
			
		}
	}
}

void Terrain::GenerateHeightMap(int mode, float* heightmap, float heightmod)
{
	//TODO: Implement generation functions
}

float Terrain::HeightAtLocation(glm::vec3 p)
{
	glm::vec3 up, dp;
	up.x = ceil(p.x);
	up.y = ceil(p.y);
	up.z = ceil(p.z);

	dp.x = floor(p.x);
	dp.y = floor(p.y);
	dp.z = floor(p.z);

	glm::vec3 verts[3];

	unsigned int ui, ud;

	/*
	find rounded down vec, find rounded up vec, those points make up the square
	determine greater value, x or z, to determine which triangle it forms
	*/

	ui = IndexAt(up.x, up.z);
	ud = IndexAt(dp.x, dp.z);

	if (up.x > dp.z)
	{
		//lower right
		verts[0] = _heightMap[ud - 1];
		verts[1] = _heightMap[ud];
		verts[2] = _heightMap[ud - _width];
	}
	else
	{
		//default upper left if issue
		verts[0] = _heightMap[ui + _width];
		verts[1] = _heightMap[ui];
		verts[2] = _heightMap[ui + 1];
	}

	rvtResponse rvt;

	if (RayVsTriangle(glm::vec3(0, -1, 0), p, verts[0], verts[1], verts[2], &rvt))
	{
		rvt.intersection.y;
	}
	else
	{
		return p.y;
	}

}