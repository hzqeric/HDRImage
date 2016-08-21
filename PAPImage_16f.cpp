#include "stdafx.h"
#include "PAPImage_16f.h"
#include <fstream>
#include <sstream>
#include "PAPException.h"

PAPImage_16f::PAPImage_16f(const unsigned short width, const unsigned short height) 
: PAPImage(width, height, IDF16fpp){
	_data = NULL;
	setDimensions(width, height);
}

PAPImage_16f::~PAPImage_16f() {
	delete _data;
}

void PAPImage_16f::setDimensions(const unsigned short width, const unsigned short height) {
	PAPImage::setDimensions(width, height);

	if (_data != NULL) {
		delete _data;
	}
	_data = new Pixel16f[width * height];
	memset(_data, 0, getDataSize()); // Paint it black.
}

void PAPImage_16f::saveToFile(std::string fileName) {
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

void PAPImage_16f::loadFromStream(std::istream& stream) {
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

float PAPImage_16f::getPixel_Red_16f(const unsigned short x, const unsigned short y) {
	return _data[getIndex(x, y)].rgbRed;
}

float PAPImage_16f::getPixel_Blue_16f(const unsigned short x, const unsigned short y) {
	return _data[getIndex(x, y)].rgbBlue;
}

float PAPImage_16f::getPixel_Green_16f(const unsigned short x, const unsigned short y) {
	return _data[getIndex(x, y)].rgbGreen;
}

void PAPImage_16f::setPixel_Red_16f(const unsigned short x, const unsigned short y, float red) {
	_data[getIndex(x, y)].rgbRed = red;
}

void PAPImage_16f::setPixel_Blue_16f(const unsigned short x, const unsigned short y, float blue) {
	_data[getIndex(x, y)].rgbBlue = blue;
}

void PAPImage_16f::setPixel_Green_16f(const unsigned short x, const unsigned short y, float green) {
	_data[getIndex(x, y)].rgbGreen = green;
}

void PAPImage_16f::setPixel_16f(const unsigned short x, const unsigned short y, Pixel16f pixel) {
	int index = (x, y);
	_data[index].rgbRed = pixel.rgbRed;
	_data[index].rgbBlue = pixel.rgbBlue;
	_data[index].rgbGreen = pixel.rgbGreen;
}

PAPImage* PAPImage_16f::convertToIDF(imageDataFormat IDF){

	// TODO
	return NULL;
}

unsigned int PAPImage_16f::getDataSize(){
	return _width * _height * sizeof(Pixel16f);
}

unsigned int PAPImage_16f::getIndex(unsigned short x, unsigned short y){
	return y * _width + x;
}