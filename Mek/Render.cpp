//=========================================================================================//
//	James Conrad - Jan 27 2016                                                             //
//  Swift Strike Studios                                                                   //
//  Standard class for drawing objects from given data                                     //
//=========================================================================================//

#include "Render.h"

Render::Render() : _numtex(0), _numvbo(0), _indexed(0)
{
	glGenVertexArrays(1, &_vao);
}

Render::~Render()
{
	destroy();
}

void Render::initModes(unsigned int m)
{
	_dflag = m;
}

void Render::destroy()
{
	
}

void Render::createVBOs(VBOData* data, unsigned int num)
{
	glBindVertexArray(_vao);
	glGenBuffers(num, _vbo);
	_numvbo = num;

	for (int i = 0; i < num; i++)
	{
		if (data[i].target != GL_ELEMENT_ARRAY_BUFFER)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[i]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data[i].elementSize * data[i].numElements, data[i].data, GL_STATIC_DRAW);
			_indexed = true;
		}
		else
		{
			glBindBuffer(data[i].target, _vbo[i]);
			glBufferData(data[i].target, data[i].elementSize * data[i].numElements, data[i].data, GL_STATIC_DRAW);
			glEnableVertexAttribArray(data[i].attribLoc);
			glVertexAttribPointer(data[i].attribLoc, data[i].numElementsPerAttrib, data[i].elementType, GL_FALSE, 0, 0);
		}
	}
	glBindVertexArray(0);
}

void Render::createTexture(char* filepath, char* target, TextureFlag usage)
{
	Tex* newtex = new Tex[_numtex + 1];
	memcpy(newtex, _textures, (_numtex) * sizeof(Tex));
	delete[] _textures;
	_textures = newtex;

	_textures[_numtex].texture = new Texture(filepath);
	_textures[_numtex].usage = usage;

	Tex tex;
	tex.texture = new Texture(filepath);
	tex.usage = usage;
	tex.target = target;
	_textures[_numtex] = tex;

	_numtex++;
}

void Render::draw()
{



	if (_indexed)
	{

	}
	else
	{

	}



}