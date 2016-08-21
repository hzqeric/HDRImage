#include "stdafx.h"
#include "PAPImage_32i.h"
#include "PAPImage_24i.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "PAPException.h"
#include <algorithm>

PAPImage_32i::PAPImage_32i(const unsigned short width, const unsigned short height)
: PAPImage(width, height, IDF32i)
{
	_data = NULL;
	setDimensions(width, height);
}

PAPImage_32i::~PAPImage_32i()
{
	delete _data;
}

void PAPImage_32i::setDimensions(const unsigned short width, const unsigned short height){
	
	if (_data != NULL) {
		delete _data;
	}	

	PAPImage::setDimensions(width, height);
	_data = new Pixel32i[width * height];
	memset(_data, 0, getDataSize()); // Paint it black.
}

unsigned int PAPImage_32i::getDataSize() {
	return sizeof(Pixel32i) * _width * _height;
}

void PAPImage_32i::loadFromStream(std::istream& stream) {
	BITMAPFILEHEADER fileHeader;
	stream.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER infoHeader;
	stream.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

	if (fileHeader.bfType != 0x4d42) {
		stringstream ss = stringstream();
		ss << "Stream doesn't doesn\'t contain BITMAP (lacks magic header). File reports " << fileHeader.bfOffBits << " (logged by PAPImage_32i).\n";
		throw PAPException(ss.str());
	}

	// Not a 32 bit BMP
	if (infoHeader.biBitCount != 32) {
		stringstream ss = stringstream();
		throw PAPException("Stream doesn\'t doesn't contain 32 bit BITMAP." );
	}

	// Verifying size and rescaling if needed.
	if ((infoHeader.biWidth != _width) || (infoHeader.biHeight != _height)) {
		cerr << "Warning: PAPImage_32i.loadFromStream: File dimensions doesn\'t match objects. File: " << infoHeader.biWidth << "x" << infoHeader.biHeight << " Object: " << _width << "x" << _height << ". Will rescale image.\n ";
		setDimensions(infoHeader.biWidth, infoHeader.biHeight);
	}
	stream.read((char*)_data, max(getDataSize(), fileHeader.bfSize));
}

PAPImage* PAPImage_32i::convertToIDF(imageDataFormat IDF) {
	PAPImage* result = NULL;
	PAPImage_24i* res24i;
	PAPImage_32i* res32i;

	switch (IDF) {
	case IDF24i:
		res24i = new PAPImage_24i(getWidth(), getHeight());
		result = res24i;
		for (int x = 0; x < getWidth(); x++) {
			for (int y = 0; y < getHeight(); y++) {
				res24i->setPixel_Red_8i(x, y, getPixel_Red_8i(x, y));
				res24i->setPixel_Blue_8i(x, y, getPixel_Blue_8i(x, y));
				res24i->setPixel_Green_8i(x, y, getPixel_Green_8i(x, y));
			}
		}

		break;
	case IDF32i:
		res32i = new PAPImage_32i(getWidth(), getHeight());
		result = res32i;
		for (int x = 0; x < getWidth(); x++) {
			for (int y = 0; y < getHeight(); y++) {
				res32i->setPixel_Red_8i(x, y, getPixel_Red_8i(x, y));
				res32i->setPixel_Blue_8i(x, y, getPixel_Blue_8i(x, y));
				res32i->setPixel_Green_8i(x, y, getPixel_Green_8i(x, y));
			}
		}

		break;
	}

	if (result == NULL) {
		stringstream ss = stringstream();
		ss << "PAPImage_32i cannot be converted into " << IDFtoStr(IDF) << ".";
		throw PAPException(ss.str());
	}
	return result;
}


void PAPImage_32i::saveToFile(std::string fileName) {
	
	BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = PAPImage::BMP_Magic;
	fileHeader.bfSize = getDataSize() + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	BITMAPINFOHEADER infoHeader;
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = getWidth();
	infoHeader.biHeight = getHeight();
	infoHeader.biPlanes = 1;
	infoHeader.biBitCount = 32;
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = 0;
	infoHeader.biXPelsPerMeter = 11800;
	infoHeader.biYPelsPerMeter = 11800;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	std::fstream file;
	file.open(fileName, std::fstream::out);
		
	file.write((char*) &fileHeader, sizeof(fileHeader));	
	file.write((char*) &infoHeader, sizeof(infoHeader));	
	file.write((char*) _data, getDataSize());

	file.close();
}

unsigned int PAPImage_32i::getIndex(unsigned short x, unsigned short y) {
	return _width * y + x;
}


unsigned char PAPImage_32i::getPixel_Red_8i(const unsigned short x, const unsigned short y) {
	return _data[getIndex(x, y)].rgbRed;
}

unsigned char PAPImage_32i::getPixel_Blue_8i(const unsigned short x, const unsigned short y) {
	return _data[getIndex(x, y)].rgbBlue;
}

unsigned char PAPImage_32i::getPixel_Green_8i(const unsigned short x, const unsigned short y) {
	return _data[getIndex(x, y)].rgbGreen;
}

////////////////// Setters for pixels
void PAPImage_32i::setPixel_Red_8i(const unsigned short x, const unsigned short y, const unsigned char red){
	_data[getIndex(x, y)].rgbRed = red;
}

void PAPImage_32i::setPixel_Blue_8i(const unsigned short x, const unsigned short y, const unsigned char blue){
	_data[getIndex(x, y)].rgbBlue = blue;
}

void PAPImage_32i::setPixel_Green_8i(const unsigned short x, const unsigned short y, const unsigned char green){
	_data[getIndex(x, y)].rgbGreen = green;
}

//////////// Red access for float values.

float PAPImage_32i::getPixel_Red_16f(const unsigned short x, const unsigned short y) {
	return float(_data[getIndex(x, y)].rgbRed) / 255.0f;
}

float PAPImage_32i::getPixel_Blue_16f(const unsigned short x, const unsigned short y) {
	return float(_data[getIndex(x, y)].rgbBlue) / 255.0f;
}

float PAPImage_32i::getPixel_Green_16f(const unsigned short x, const unsigned short y) {
	return float(_data[getIndex(x, y)].rgbGreen) / 255.0f;
}

