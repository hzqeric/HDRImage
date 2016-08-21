#include "stdafx.h"
#include "PAPImage.h"
#include "PAPImage_32i.h"
#include "PAPImage_24i.h"
#include "PAPImage_16f.h"
#include "PAPException.h"

#include <fstream>

std::string IDFtoStr(imageDataFormat IDF) {
	switch (IDF) {
	case IDF24i: 
		return "IDF24i";
		break;
	case IDF32i: 
		return "IDF32i";
		break;
	case IDF16fpp:
		return "IDF16f";
		break;
	case IDF32fpp:
		return "IDF32f";
		break;
	case IDF64fpp:
		return "IDF64f";
		break;
	}
	return "(unknown imageDataFormat)";
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PAPImageFactory
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PAPImageFactory::PAPImageFactory() {
	
}

PAPImageFactory::~PAPImageFactory() {
	
}

PAPImage* PAPImageFactory::createImage(const unsigned short width, const unsigned short height, const imageDataFormat IDF) {

	PAPImage* result = NULL;

	switch (IDF) {
	case IDF32i:
		result = new PAPImage_32i(width, height);
		break;
	case IDF16fpp:

		break;
	case IDF32fpp:

	case IDF64fpp:

		break;
	}

	return result;
}

PAPImage* PAPImageFactory::loadFromFile(std::string fileName) {
	std::filebuf imageFile;
	imageFile.open(fileName.c_str(), std::fstream::in);

	std::istream _istream(&imageFile);
	return loadFromStream(_istream);

	imageFile.close();
}

PAPImage* PAPImageFactory::loadFromStream(std::istream& stream) {
	// Try to read it as a BMP file.	
	BITMAPFILEHEADER fileHeader;
	stream.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER infoHeader;
	stream.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));

	stream.seekg(0);

	if (fileHeader.bfType == 0x4d42) {
		// file could be a BMP file (likely it is).
			
		if (infoHeader.biBitCount == 32) {
			PAPImage_32i* bmp32 = new PAPImage_32i(infoHeader.biWidth, infoHeader.biHeight);
			
			bmp32->loadFromStream(stream);
			return bmp32;
		}

		if (infoHeader.biBitCount == 24) {
			PAPImage_24i* bmp24 = new PAPImage_24i(infoHeader.biWidth, infoHeader.biHeight);

			bmp24->loadFromStream(stream);
			return bmp24;
		}
		// insert code for reading 8, 16 and 24 bit bitmaps here...

		// //
		
	}
	// attempt to read in HDR format instead.
	HDRHEADER HdrHeader;
	stream.read((char*)&HdrHeader, sizeof(HdrHeader));
	stream.seekg(0);

	if (HdrHeader.hhType == PAPImage::HDRmagic) {
		if ((HdrHeader.hhColourComponentBitCount == 16) && (HdrHeader.hhChannelCount == 3)) {
			PAPImage_16f* res16f = new PAPImage_16f(HdrHeader.hhWidth, HdrHeader.hhHeight);
			res16f->loadFromStream(stream);
			return res16f;
		}
	}


	throw PAPException("Failed to load from file.");
	return NULL;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// PAPImage
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PAPImage::PAPImage(const unsigned short width, const unsigned short height, const imageDataFormat IDF) {
	_width = width;
	_height = height;
	_IDF = IDF;
}

PAPImage::~PAPImage() {

}

void PAPImage::setDimensions(const unsigned short width, const unsigned short height) {
	_width = width;
	_height = height;
}


unsigned short PAPImage::getWidth() {
	return _width;
}

unsigned short PAPImage::getHeight() {
	return _height;
}

imageDataFormat PAPImage::getImageDataFormat() {
	return _IDF;
}



