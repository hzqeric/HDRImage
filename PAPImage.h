#pragma once
#include <string>

enum imageDataFormat { IDF24i, IDF32i, IDF16fpp, IDF32fpp, IDF64fpp};

// Bitmap headers
# pragma pack (1)
typedef struct tagBITMAPFILEHEADER {
	unsigned __int16 bfType;
	unsigned __int32 bfSize;
	unsigned __int16 bfReserved1;
	unsigned __int16 bfReserved2;
	unsigned __int32 bfOffBits;
} BITMAPFILEHEADER;
# pragma pack ()

# pragma pack (1)
typedef struct tagBITMAPINFOHEADER {
	unsigned __int32 biSize;
	__int32			 biWidth;
	__int32			 biHeight;
	unsigned __int16 biPlanes;
	unsigned __int16 biBitCount;
	unsigned __int32 biCompression;
	unsigned __int32 biSizeImage;
	__int32			 biXPelsPerMeter;
	__int32			 biYPelsPerMeter;
	unsigned __int32 biClrUsed;
	unsigned __int32 biClrImportant;
} BITMAPINFOHEADER;
# pragma pack ()

std::string IDFtoStr(imageDataFormat IDF);

class PAPImage
{
public:	
	PAPImage(const unsigned short width, const unsigned short height, const imageDataFormat IDF);
	virtual ~PAPImage();
	/*Resets the image size. Implementation are required to reallocate for new size.*/
	virtual void setDimensions(const unsigned short width, const unsigned short height);
	/*Returns the width of the image in pixels.*/
	unsigned short getWidth();
	/*Returns the height of the image in pixels.*/
	unsigned short getHeight();
	/*Saves the image in whatever format it currently has.*/
	virtual void saveToFile(std::string fileName) = 0;
	/*Load the data from a istream.*/
	virtual void loadFromStream(std::istream& stream) = 0;
	
	imageDataFormat getImageDataFormat();
	/* Returns a clamped float of the red channel at the pixel(x,y). */
	virtual float getPixel_Red_16f(const unsigned short x, const unsigned short y) = 0;
	/* Returns a clamped float of the blue channel at the pixel(x,y). */
	virtual float getPixel_Blue_16f(const unsigned short x, const unsigned short y) = 0;
	/* Returns a clamped float of the green channel at the pixel(x,y). */
	virtual float getPixel_Green_16f(const unsigned short x, const unsigned short y) = 0;
		
	virtual PAPImage* convertToIDF(imageDataFormat IDF) = 0;
protected:
	unsigned short _width;
	unsigned short _height;
	imageDataFormat _IDF;
};

class PAPImageFactory {
public:
	PAPImageFactory();
	virtual ~PAPImageFactory();
	PAPImage* createImage(const unsigned short width, const unsigned short height, const imageDataFormat IDF);
	PAPImage* loadFromFile(std::string fileName);
	PAPImage* loadFromStream(std::istream& stream);
};
