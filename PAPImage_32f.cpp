#include "stdafx.h"
#include "PAPImage_32f.h"
#include <fstream>
#include <sstream>
#include "PAPException.h"

PAPImage_32f::PAPImage_32f(const unsigned short width, const unsigned short height) 
: PAPImage(width, height, IDF32fpp){
	_data = NULL;
	setDimensions(width, height);
}

PAPImage_32f::~PAPImage_32f() {
	delete _data;
}

void PAPImage_32f::setDimensions(const unsigned short width, const unsigned short height) {
	PAPImage::setDimensions(width, height);

	if (_data != NULL) {
		delete _data;
	}
	_data = new Pixel16f[width * height];
	memset(_data, 0, getDataSize()); // Paint it black.
}

void PAPImage_32f::saveToFile(std::string fileName) {
	HDRHEADER HDRHeader;
	HDRHeader.hhType = HDRmagic;
	HDRHeader.hhChannelCount = 3;
	HDRHeader.hhColourComponentBitCount = 16;
	HDRHeader.hhHeaderSize = sizeof(HDRHeader);
	HDRHeader.hhFileSize = sizeof(HDRHeader) + getDataSize();
	HDRHeader.hhWidth = getWidth();
	HDRHeader.hhHeight = getHeight();
	
	std::fstream file;
	file.open(fileName, std::fstream::out);
	file.write((char*)&HDRHeader, sizeof(HDRHeader));	
	file.write((char*)_data, getDataSize());
	file.close();
}

void PAPImage_32f::loadFromStream(std::ifstream& stream) {
	HDRHEADER HDRHeader;
	stream.read((char*)&HDRHeader, sizeof(HDRHeader));

	if (HDRHeader.hhType != PAPImage::HDRmagic) {
		throw PAPException("Stream doesn\'t contain 16b pp image.");
	}

	if ((HDRHeader.hhWidth != _width) | (HDRHeader.hhHeight != _height)) {
		cerr << "Warning: PAPImage_16f.loadFromStream: File dimensions doesn\'t match objects. File: " << HDRHeader.hhWidth << "x" << HDRHeader.hhHeight << " Object: " << _width << "x" << _height << ". Will rescale image.\n ";
		setDimensions(HDRHeader.hhWidth, HDRHeader.hhHeight);
	}
	stream.read((char*)_data, getDataSize());
}

float PAPImage_32f::getPixel_Red_16f(const unsigned short x, const unsigned short y) const{
	return _data[getIndex(x, y)].rgbRed;
}

float PAPImage_32f::getPixel_Blue_16f(const unsigned short x, const unsigned short y) const{
	return _data[getIndex(x, y)].rgbBlue;
}

float PAPImage_32f::getPixel_Green_16f(const unsigned short x, const unsigned short y) const{
	return _data[getIndex(x, y)].rgbGreen;
}

void PAPImage_32f::setPixel_Red_16f(const unsigned short x, const unsigned short y, float red) {
	_data[getIndex(x, y)].rgbRed = red;
}

void PAPImage_32f::setPixel_Blue_16f(const unsigned short x, const unsigned short y, float blue) {
	_data[getIndex(x, y)].rgbBlue = blue;
}

void PAPImage_32f::setPixel_Green_16f(const unsigned short x, const unsigned short y, float green) {
	_data[getIndex(x, y)].rgbGreen = green;
}

void PAPImage_32f::setPixel_16f(const unsigned short x, const unsigned short y, Pixel16f pixel) {
	int index = (x, y);
	_data[index].rgbRed = pixel.rgbRed;
	_data[index].rgbBlue = pixel.rgbBlue;
	_data[index].rgbGreen = pixel.rgbGreen;
}

unsigned int PAPImage_32f::getDataSize() const{
	return _width * _height * sizeof(Pixel16f);
}

unsigned int PAPImage_32f::getIndex(unsigned short x, unsigned short y) const{
	return y * _width + x;
}