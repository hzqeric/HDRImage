#include "stdafx.h"
#include "PAPImage.h"
#include "PAPImage_8i.h"
#include "PAPImage_32f.h"
#include "PAPException.h"

#include <fstream>

std::string IDFtoStr(imageDataFormat IDF) {
	switch (IDF) {
	case IDF8i3: 
		return "IDF8i3";
		break;
	case IDF8i4: 
		return "IDF8i4";
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

imageDataFormat PAPImage::getImageDataFormat() const{
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
	case IDF8i3:		
	case IDF8i4:
		result = new PAPImage_8i(width, height, IDF);
		break;
	case IDF32fpp:

		break;
	case IDF64fpp:

		break;
	}

	return result;
}

PAPImage* PAPImageFactory::createImageFromFile(std::string fileName) {	
	std::ifstream _istream(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
	PAPImage* result = createImageFromStream(_istream);	
	_istream.close();
	return result;
}

PAPImage* PAPImageFactory::createImageFromStream(std::ifstream& stream) {

	PAPImage_8i* img8;
	std::streamoff startPos = stream.tellg();

	// Try to read it as a BMP file.	
	BITMAPFILEHEADER fileHeader;
	stream.read((char*)&fileHeader, sizeof(BITMAPFILEHEADER));

	BITMAPINFOHEADER infoHeader;
	stream.read((char*)&infoHeader, sizeof(BITMAPINFOHEADER));
	stream.seekg(startPos);

	if (fileHeader.bfType == 0x4d42) {
		// file could be a BMP file (likely it is).
		
		if (infoHeader.biBitCount == 32) {
			img8 = new PAPImage_8i(infoHeader.biWidth, infoHeader.biHeight, IDF8i4);
		}
		else
			if (infoHeader.biBitCount == 24) {
				img8 = new PAPImage_8i(infoHeader.biWidth, infoHeader.biHeight, IDF8i3);
			}  	// insert code for reading 8bit and 16bit bitmaps here...
			else {
				throw PAPException("unsupported bitmap size.");
			}
			img8->loadFromStream(stream);
			return img8;
	}
	
	// attempt to read in HDR format instead.
	HDRHEADER HdrHeader;
	stream.read((char*)&HdrHeader, sizeof(HdrHeader));
	stream.seekg(startPos);

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

void convertIDF24into(const imageDataFormat targetIDF, const PAPImage_8i& src, PAPImage& target) {
	PAPImage_8i* target24i = (PAPImage_8i*) &target;
	
	switch (target.getImageDataFormat()) {
	case IDF8i3:	
	case IDF8i4:
		for (unsigned short x = 0; x < src.getWidth(); x++) {
			for (unsigned short y = 0; y < src.getHeight(); y++) {
				target24i->setPixel_Red_8i(x, y, src.getPixel_Red_8i(x, y));
				target24i->setPixel_Blue_8i(x, y, src.getPixel_Blue_8i(x, y));
				target24i->setPixel_Green_8i(x, y, src.getPixel_Green_8i(x, y));
			}
		}
		
		break;		
	case IDF32fpp:
		break;
	case IDF64fpp:
		break;
	}
}


PAPImage* PAPImageFactory::createImageFromImage(const PAPImage& src, const imageDataFormat targetIDF) {
	PAPImage* result = NULL;
		
	// Initialise target image.
	switch (targetIDF) {
	case IDF8i3: 
	case IDF8i4: result = new PAPImage_8i(src.getWidth(), src.getHeight(), targetIDF);
		break;
	case IDF32fpp: result = new PAPImage_32f(src.getWidth(), src.getHeight());
		break;
	// case IDF64fpp: = new PAPImage_64f(src.getWidth(), src.getHeight());
	// break;
	}

	switch (src.getImageDataFormat()) {
	case IDF8i3:		
	case IDF8i4: convertIDF24into(targetIDF, *((PAPImage_8i*)&src), *result);
		break;
	}
		
	return result;
}


