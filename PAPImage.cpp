#include "stdafx.h"
#include "PAPImage.h"
#include "PAPImage_32i.h"
#include "PAPImage_24i.h"
#include "PAPImage_32f.h"
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
	case IDF32fpp:
		return "IDF16f";
		break;
	case IDF64fpp:
		return "IDF32f";
		break;	
	}
	return "(unknown imageDataFormat)";
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


unsigned short PAPImage::getWidth() const{
	return _width;
}

unsigned short PAPImage::getHeight() const{
	return _height;
}

imageDataFormat PAPImage::getImageDataFormat() {
	return _IDF;
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
	case IDF24i:

		break;
	case IDF32i:
		result = new PAPImage_32i(width, height);
		break;
	case IDF32fpp:

		break;
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
			PAPImage_32f* res16f = new PAPImage_32f(HdrHeader.hhWidth, HdrHeader.hhHeight);
			res16f->loadFromStream(stream);
			return res16f;
		}
	}


	throw PAPException("Failed to load from file.");
	return NULL;
}

void convertIDF24into(const imageDataFormat targetIDF, const PAPImage_24i& src, PAPImage& target) {
	PAPImage_24i* target24i = (PAPImage_24i*)&target;
	PAPImage_32i* target32i = (PAPImage_32i*)&target;

	switch (target.getImageDataFormat()) {
	case IDF24i:	
		for (unsigned short x = 0; x < src.getWidth(); x++) {
			for (unsigned short y = 0; y < src.getHeight(); y++) {
				target24i->setPixel_Red_8i(x, y, src.getPixel_Red_8i(x, y));
				target24i->setPixel_Blue_8i(x, y, src.getPixel_Blue_8i(x, y));
				target24i->setPixel_Green_8i(x, y, src.getPixel_Green_8i(x, y));
			}
		}
		break;
	case IDF32i:		
		for (unsigned short x = 0; x < src.getWidth(); x++) {
			for (unsigned short y = 0; y < src.getHeight(); y++) {
				target32i->setPixel_Red_8i(x, y, src.getPixel_Red_8i(x, y));
				target32i->setPixel_Blue_8i(x, y, src.getPixel_Blue_8i(x, y));
				target32i->setPixel_Green_8i(x, y, src.getPixel_Green_8i(x, y));
			}
		}
		break;
	case IDF32fpp:
		break;
	case IDF64fpp:
		break;
	}

}

void convertIDF32iinto(const imageDataFormat targetIDF, const PAPImage_32i& src, PAPImage& target){
	PAPImage_24i* target24i = (PAPImage_24i*)&target;
	PAPImage_32i* target32i = (PAPImage_32i*)&target;

	switch (target.getImageDataFormat()) {
	case IDF24i:		
		for (unsigned short x = 0; x < src.getWidth(); x++) {
			for (unsigned short y = 0; y < src.getHeight(); y++) {
				target24i->setPixel_Red_8i(x, y, src.getPixel_Red_8i(x, y));
				target24i->setPixel_Blue_8i(x, y, src.getPixel_Blue_8i(x, y));
				target24i->setPixel_Green_8i(x, y, src.getPixel_Green_8i(x, y));
			}
		}
		break;
	case IDF32i:		
		for (unsigned short x = 0; x < src.getWidth(); x++) {
			for (unsigned short y = 0; y < src.getHeight(); y++) {
				target32i->setPixel_Red_8i(x, y, src.getPixel_Red_8i(x, y));
				target32i->setPixel_Blue_8i(x, y, src.getPixel_Blue_8i(x, y));
				target32i->setPixel_Green_8i(x, y, src.getPixel_Green_8i(x, y));
			}
		}
		break;
	case IDF32fpp:
		break;
	case IDF64fpp:
		break;
	}

}


PAPImage* PAPImageFactory::createImageFromImage(PAPImage& src, const imageDataFormat targetIDF) {

	PAPImage* result = NULL;
	PAPImage_24i img24 = *((PAPImage_24i*)&src);
	
	// Initialise target image.
	switch (targetIDF) {
	case IDF24i: result = new PAPImage_24i(src.getWidth(), src.getHeight());
		break;
	case IDF32i: result = new PAPImage_32i(src.getWidth(), src.getHeight());
		break;
	case IDF32fpp: result = new PAPImage_32f(src.getWidth(), src.getHeight());
		break;
	// case IDF64fpp: = new PAPImage_64f(src.getWidth(), src.getHeight());
	// break;
	}

	switch (src.getImageDataFormat()) {
	case IDF24i: convertIDF24into(targetIDF, *((PAPImage_24i*)&src), *result);
		break;
	case IDF32i: convertIDF32iinto(targetIDF, *((PAPImage_32i*)&src), *result);
		break;
	}
		
	return result;
}


