//=========================================================================================//
//	James Conrad - Jan 27 2016                                                             //
//  Swift Strike Studios                                                                   //
//  Standard class for drawing objects from given data                                     //
//=========================================================================================//
#pragma once
#include "Program.h"
#include "Texture.h"

//Draw flags
enum DrawFlag
{
	NORMAL_MAP = 0x01,
	BUMP_MAP = 0x02,
	LIGHTING_STANDARD = 0x04,
	SHADOWS = 0x08
};

//Texture flags
enum TextureFlag
{
	NONE		= 0,
	DIFFUSE		,
	SPECULAR	,
	AMBIENT		,
	EMISSIVE	,
	HEIGHT		,
	NORMALS		,
	SHININESS	,
	OPACITY		,//DO NOT USE
	DISPLACEMENT,
	LIGHTMAP	,
	REFLECTION	
};

struct VBOData
{
	unsigned int attribLoc;				//Layout location
	GLenum elementType;					//Type of data (GL_FLOAT)
	GLenum target;						//Target buffer to bind to
	unsigned int elementSize;			//Size (in bytes) per element
	unsigned int numElementsPerAttrib;	//Number of elements per attribute
	unsigned int numElements;			//Number of elements in data
	void* data;							//Data pointer
	//note on interleaving
	//stride = total elementsize
	//pointer = start + total elementsize
};

class Render
{
public:
	Render();
	~Render();
	void destroy();

	//Bitwise flags used,
	//Example: initModes(OPT_1 | OPT_2);
	//Available options are inside enum DrawFlag
	void initModes(unsigned int); //OR (|) to set, AND (&) to check

	//Must be called before createVBO
	void prepareVBOs(unsigned int);

	
	//Must be called after prepareVBOs
	void createVBOs(VBOData* data, unsigned int num);

	bool createTexture(char* filepath, char* target, TextureFlag usage);

	void draw(bool basevertex = false, unsigned int baseIndex = 0, unsigned int baseVertex = 0);
	void drawShadowPass(bool basevertex = false, unsigned int baseIndex = 0, unsigned int baseVertex = 0);

	int numTextures();

	//This function should be avoided, however was put in for the model class usage.
	void forceOverride(
		unsigned int vao,
		unsigned int* vbo,
		unsigned int numvbo,
		unsigned int  dflag,	//currently unused
		bool indexed,
		unsigned int numindices,
		unsigned int numvertices);

	GLuint renderedTexture = 0;
	inline void setRenderedTexture(GLuint &_passedTexture){ renderedTexture = _passedTexture; }

private:
	struct Tex
	{
		TextureFlag usage;
		char* target;
		Texture* texture;
	};

	unsigned int _vao;
	unsigned int* _vbo;
	unsigned int _numvbo;
	unsigned int  _dflag;
	bool _indexed;
	unsigned int _numindices;
	unsigned int _numvertices;
	Tex* _textures;

	unsigned int _numtex;
};