#pragma once

#include <string>
#include "include\IL\il.h"

class Bitmap {
public:
	enum Format {
		Format_Grayscale = 1, /**< one channel: grayscale */
		Format_GrayscaleAlpha = 2, /**< two channels: grayscale and alpha */
		Format_RGB = 3, /**< three channels: red, green, blue */
		Format_RGBA = 4 /**< four channels: red, green, blue, alpha */
	};

	Bitmap();
	~Bitmap();

	bool bitmapFromFile(std::string filePath, int *format = nullptr);

	unsigned width() const;

	unsigned height() const;

	Format format() const;

	unsigned char* pixelBuffer() const;

	unsigned char* getPixel(unsigned int column, unsigned int row) const;

	void setPixel(unsigned int column, unsigned int row, const unsigned char* pixel);

	void flipVertically();

	void rotate90CounterClockwise();

	void copyRectFromBitmap(const Bitmap& src,
		unsigned srcCol,
		unsigned srcRow,
		unsigned destCol,
		unsigned destRow,
		unsigned width,
		unsigned height);

	Bitmap(const Bitmap& other);

	Bitmap& operator = (const Bitmap& other);

	unsigned char* _pixels;
private:
	ILuint _image;
	Format _format;
	unsigned _width;
	unsigned _height;

	void _set(unsigned width, unsigned height, Format format, const unsigned char* pixels);
	static void _getPixelOffset(unsigned col, unsigned row, unsigned width, unsigned height, Format format);
};
