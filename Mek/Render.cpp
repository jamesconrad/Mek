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

int Render::numTextures()
{
	return _numtex;
}

void Render::createVBOs(VBOData* data, unsigned int num)
{
	glBindVertexArray(_vao);
	_vbo = new unsigned int[num];
	glGenBuffers(num, _vbo);
	_numvbo = num;

	for (int i = 0; i < num; i++)
	{
		if (data[i].target == GL_ELEMENT_ARRAY_BUFFER)
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

bool Render::createTexture(char* filepath, char* target, TextureFlag usage)
{
	Texture* tmp = new Texture(filepath);
	if (!(tmp->originalHeight() > 0))
		return false;
	Tex* newtex = new Tex[_numtex + 1];
	memcpy(newtex, _textures, (_numtex)* sizeof(Tex));
	delete[] _textures;
	_textures = newtex;

	_textures[_numtex].texture = tmp;
	_textures[_numtex].usage = usage;

	Tex tex;
	tex.texture = new Texture(filepath);
	tex.usage = usage;
	tex.target = target;
	_textures[_numtex] = tex;

	_numtex++;
	return true;
}

void Render::draw(bool basevertex, unsigned int baseIndex, unsigned int baseVertex)
{
	glBindVertexArray(_vao);
	//img binding
	for (int i = 0; i < _numtex; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, _textures[i].texture->object());
		Program::getInstance().setUniform(_textures[i].target, i);
	}
	if (renderedTexture != 0)
	{
		
		glBindTexture(GL_TEXTURE_2D, renderedTexture);
	}
	if (_indexed && basevertex)
		glDrawElementsBaseVertex(GL_TRIANGLES, _numindices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*baseIndex), baseVertex);
	else if (_indexed)
		glDrawElements(GL_TRIANGLES, _numindices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, _numvertices);
	glBindVertexArray(0);
}

void Render::drawShadowPass(bool basevertex, unsigned int baseIndex, unsigned int baseVertex)
{
	glBindVertexArray(_vao);
	if (_indexed && basevertex)
		glDrawElementsBaseVertex(GL_TRIANGLES, _numindices, GL_UNSIGNED_INT, (void*)(sizeof(unsigned int)*baseIndex), baseVertex);
	else if (_indexed)
		glDrawElements(GL_TRIANGLES, _numindices, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, _numvertices);

	//Program::getInstance().unbind();
	glBindVertexArray(0);
}

void Render::forceOverride(
	unsigned int vao,
	unsigned int* vbo,
	unsigned int numvbo,
	unsigned int  dflag,
	bool indexed,
	unsigned int numindices,
	unsigned int numvertices)
{
	_vao = vao;
	_vbo = new unsigned int[numvbo];
	for (int i = 0; i < numvbo; i++)
		_vbo[i] = vbo[i];
	_numvbo = numvbo;
	_dflag = dflag;
	_indexed = indexed;
	_numindices = numindices;
	_numvertices = numvertices;
}