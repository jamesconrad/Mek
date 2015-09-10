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

Texture::Texture(const Bitmap& bitmap, GLint minMagFiler, GLint wrapMode) :
_originalWidth((GLfloat)bitmap.width()),
_originalHeight((GLfloat)bitmap.height())
{
	ilutGLLoadImage("C:/Users/100559437/Desktop/Mek/Debug/wooden-crate.jpg");
	ILenum err = ilGetError();
	//_object = ilutGLBindTexImage();
	//err = ilGetError();
	//ilutGLTexImage(0);
	//err = ilGetError();

	//_originalWidth = ilGetInteger(IL_IMAGE_WIDTH);
	//_originalHeight = ilGetInteger(IL_IMAGE_HEIGHT);

	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, minMagFiler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		TextureFormatForBitmapFormat(bitmap.format(), true),
		(GLsizei)bitmap.width(),
		(GLsizei)bitmap.height(),
		0,
		TextureFormatForBitmapFormat(bitmap.format(), false),
		GL_UNSIGNED_BYTE,
		bitmap.pixelBuffer());
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
