#include "include\GL\glew.h"
#include "lib\glm\common.hpp"
#include "Texture.h"
#include "Camera.h"
#include "Shader.h"

class Skybox
{
public:
	//easy way of use is Skybox({"Right", "Left", "Top", "Bottom", "Back", "Front"});
	Skybox(char* fp[6], char* op[6], char* sp[6]);

	void render(bool ObsucityMap);

private:
	GLuint _cubeMapID;
	GLuint _vao, _vbo;
	Bitmap _bmp[6];
	GLuint _obscureMapID;
	Bitmap _obscureBMP[6];
	GLuint _staticMapID;
	Bitmap _staticBMP[6];
};