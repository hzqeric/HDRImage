#include "stdafx.h"
#include "PAPImage_32f.h"
#include "PAPImage_8i.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include "PAPException.h"

unsigned short chRed = 2;
unsigned short chBlue = 0;
unsigned short chGreen = 1;

PAPImage_8i::PAPImage_8i(const unsigned short width, const unsigned short height, const imageDataFormat IDF)
	: PAPImage(width, height, IDF)
{
	if (!((IDF == IDF8i3) || (IDF == IDF8i4))) {
		throw PAPException("bad arguments to image creation. Invalid image format.");
	}

	if (IDF == IDF8i3) {
		_channelCount = 3;
	}
	else 
		if (IDF == IDF8i4) {
			_channelCount = 4;
		}

	_data = NULL;
	setDimensions(width, height);
}

PAPImage_8i::~PAPImage_8i()
{
	delete _data;
}

void PAPImage_8i::setDimensions(const unsigned short width, const unsigned short height){
	if (_data != NULL) {
		delete _data;
	}
	PAPImage::setDimensions(width, height);
	
	if (((_channelCount * _width) % 4) == 0) {  // Rowsize is expressed in bytes
		_rowSize = _channelCount * _width;
	}
	else {
		_rowSize = _channelCount * _width + (4 - (_channelCount * _width ) % 4);
	}
	
	_dataSize = sizeof(unsigned __int8) * _rowSize * _height;
	_data = new unsigned __int8[_channelCount * _rowSize * _height];	
	memset(_data, 0, _dataSize); // Paint it black.
}

/** Returns index in memory of red channel byte of a pixel.*/
unsigned int PAPImage_8i::getIndex(unsigned short x, unsigned short y) const {
	return _rowSize * y + _channelCount * x;
}

// Getters for pixels.
unsigned char PAPImage_8i::getPixel_Red_8i(const unsigned short x, const unsigned short y) const {
	return _data[getIndex(x, y) + chRed];
}

unsigned char PAPImage_8i::getPixel_Blue_8i(const unsigned short x, const unsigned short y) const {
	return _data[getIndex(x, y) + chBlue];
}

unsigned char PAPImage_8i::getPixel_Green_8i(const unsigned short x, const unsigned short y) const {
	return _data[getIndex(x, y) + chGreen];
}

////////////////// Setters for pixels
void PAPImage_8i::setPixel_Red_8i(const unsigned short x, const unsigned short y, const unsigned char red) {
	_data[getIndex(x, y) + chRed] = red;
}

void PAPImage_8i::setPixel_Blue_8i(const unsigned short x, const unsigned short y, const unsigned char blue) {
	_data[getIndex(x, y) + chBlue] = blue;
}

void PAPImage_8i::setPixel_Green_8i(const unsigned short x, const unsigned short y, const unsigned char green) {
	_data[getIndex(x, y) + chGreen] = green;
}


void PAPImage_8i::saveToFile(std::string fileName) {
	BITMAPFILEHEADER fileHeader;
	fileHeader.bfType = PAPImage::BMP_Magic;
	fileHeader.bfSize = _dataSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	fileHeader.bfReserved1 = 0;
	fileHeader.bfReserved2 = 0;
	fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER infoHeader;
	infoHeader.biSize = sizeof(BITMAPINFOHEADER);
	infoHeader.biWidth = getWidth();
	infoHeader.biHeight = getHeight();
	infoHeader.biPlanes = 1;
	if (_IDF == IDF8i3) {
		infoHeader.biBitCount = 24;
	}
	else
		if (_IDF == IDF8i4) {
			infoHeader.biBitCount = 32;
		}
	infoHeader.biCompression = 0;
	infoHeader.biSizeImage = 0; // Ok to set to 0 for uncompressed images.
	infoHeader.biXPelsPerMeter = 11800;
	infoHeader.biYPelsPerMeter = 11800;
	infoHeader.biClrUsed = 0;
	infoHeader.biClrImportant = 0;

	std::ofstream file;
	file.open(fileName, std::ofstream::out | std::ofstream::binary);
	file.write((char*)&fileHeader, sizeof(fileHeader));
	file.write((char*)&infoHeader, sizeof(infoHeader));
	file.write((char*)_data, _dataSize);

	file.close();
}

void PAPImage_8i::loadFromStream(std::ifstream& stream) {
	BITMAPFILEHEADER fileHeader;
	stream.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER infoHeader;

	if (fileHeader.bfOffBits < sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) {
		cerr << "Warning: Older bitmap header format in file. Attempting to read anyway.\n";
	}
	stream.read((char*) &infoHeader, sizeof(BITMAPINFOHEADER));

	if (fileHeader.bfType != 0x4d42) {
		stringstream ss = stringstream();
		ss << "Stream doesn't doesn\'t contain BITMAP (lacks magic header). File reports " << fileHeader.bfOffBits << " (logged by PAPImage_24i) .\n";
		throw PAPException(ss.str());
	}

	if ((_IDF == IDF8i3) && (infoHeader.biBitCount != 24)) {
		throw PAPException("Stream doesn\'t doesn't contain a compatible BITMAP (24 bit).");
	}

	if ((_IDF == IDF8i4) && (infoHeader.biBitCount != 32)) {
		throw PAPException("Stream doesn\'t doesn't contain a compatible BITMAP (32 bit).");
	}

	// Verifying size and rescaling if needed.
	if ((infoHeader.biWidth != _width) || (infoHeader.biHeight != _height)) {
		cerr << "Warning: PAPImage_8i.loadFromStream: File dimensions doesn\'t match objects. File: " << infoHeader.biWidth << "x" << infoHeader.biHeight << " Object: " << _width << "x" << _height << ". Will rescale image.\n ";
		setDimensions(infoHeader.biWidth, infoHeader.biHeight);
	}

	stream.seekg(fileHeader.bfOffBits);
	
	stream.read((char*)_data, min(_dataSize, infoHeader.biSizeImage));
	if (!stream) {
		cerr << "Warning: failed to read stream. " << stream.gcount() << " bytes read.\n";
		cerr << "Datasize: " << _dataSize << endl;
		cerr << "bSizeImage: " << infoHeader.biSizeImage << endl;
	}
}
