#include "Texture.h"
#include <stdexcept>
#include "include\IL\ilut.h"

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format, bool srgb)
{
	switch (format) {
	case Bitmap::Format_Grayscale: return GL_LUMINANCE;
	case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
	case Bitmap::Format_RGB: return (srgb ? GL_SRGB : GL_RGB);
	case Bitmap::Format_RGBA: return (srgb ? GL_SRGB_ALPHA : GL_RGBA);
	default: throw std::runtime_error("Unrecognised Bitmap::Format");
	}
}

Texture::Texture(char* filepath, GLint minMagFiler, GLint wrapMode)
{
	Bitmap bmp;
	bmp.bitmapFromFile(filepath);
	bmp.flipVertically();

	_originalHeight = bmp.height();
	_originalWidth = bmp.width();

	GLenum err = glGetError();

	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);
	err = glGetError();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	err = glGetError();
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGB,
		_originalWidth,
		_originalHeight,
		0,
		GL_RGB,
		GL_UNSIGNED_BYTE,
		bmp.pixelBuffer());
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &_object);
}

GLuint Texture::object() const
{
	return _object;
}

GLfloat Texture::originalWidth() const
{
	return _originalWidth;
}

GLfloat Texture::originalHeight() const
{
	return _originalHeight;
}
