//=========================================================================================//
//	James Conrad - Jan 27 2016                                                             //
//  Swift Strike Studios                                                                   //
//  Standard class for drawing objects from given data                                     //
//=========================================================================================//

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
	IMAGE = 0x01,
	NORMAL = 0x02,
	BUMP = 0x04
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

	void createTexture(char* filepath, char* target, TextureFlag usage);

	void draw();
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
	Tex* _textures;
	unsigned int _numtex;
};