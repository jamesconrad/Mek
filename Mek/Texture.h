#pragma once

#include "include\GL\glew.h"
#include "Bitmap.h"
#include <map>

class Texture {
public:
	Texture(char* file,
		bool mip = true,
		GLint minMagFiler = GL_LINEAR,
		GLint wrapMode = GL_CLAMP_TO_EDGE);

	~Texture();

	GLuint object() const;

	GLfloat originalWidth() const;

	GLfloat originalHeight() const;

private:
	GLuint _object;
	GLfloat _originalWidth;
	GLfloat _originalHeight;

	//copying disabled
	Texture(const Texture&);
	const Texture& operator=(const Texture&);

	static std::map<char*, Texture*> imgMap;
};