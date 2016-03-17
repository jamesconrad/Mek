#include "Texture.h"
#include <stdexcept>
#include "include\IL\ilut.h"

std::map<char*, Texture*> Texture::imgMap;

static GLenum TextureFormatForBitmapFormat(Bitmap::Format format, bool srgb)
{
	switch (format) {
	case Bitmap::Format_Grayscale: return GL_LUMINANCE;
	case Bitmap::Format_GrayscaleAlpha: return GL_LUMINANCE_ALPHA;
	case Bitmap::Format_RGB: return (srgb ? GL_SRGB : GL_RGB);
	case Bitmap::Format_RGBA: return (srgb ? GL_SRGB_ALPHA : GL_RGBA);
	//default: throw std::runtime_error("Unrecognised Bitmap::Format");
	default: return GL_RGBA;
	}
}

Texture::Texture(char* filepath, bool mip, GLint minMagFiler, GLint wrapMode)
{
	if (imgMap.find(filepath) != imgMap.end())
	{
		//we found a copy of ourself
		_object = imgMap.at(filepath)->_object;
		_originalHeight = imgMap.at(filepath)->_originalHeight;
		_originalWidth = imgMap.at(filepath)->_originalWidth;
		return;
	}
	imgMap.emplace(filepath, this);

	std::string debfp = "../Debug/";
	debfp.append(filepath);
	filepath = (char*)debfp.c_str();

	Bitmap bmp;
	bmp.bitmapFromFile(filepath);
	bmp.flipVertically();

	_originalHeight = bmp.height();
	_originalWidth = bmp.width();

	GLenum err = glGetError();

	GLenum imgType = TextureFormatForBitmapFormat(bmp.format(), false);
	
	glGenTextures(1, &_object);
	glBindTexture(GL_TEXTURE_2D, _object);
	glTexImage2D(GL_TEXTURE_2D, 0, imgType, _originalWidth, _originalHeight, 0, imgType, GL_UNSIGNED_BYTE, bmp.pixelBuffer());
	if (mip)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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