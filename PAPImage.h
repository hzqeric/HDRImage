#pragma once
#include <string>

enum imageDataFormat {IDF24i, IDF32i, IDF32fpp, IDF64fpp};

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

# pragma pack (1)
typedef struct tagHDRHEADER {
	unsigned __int32 hhType;					// Magic number 0x7675 
	unsigned __int32 hhHeaderSize;				// Size of headers, or what position the image data starts.
	unsigned __int32 hhFileSize;				// Size of file, including header.
	unsigned __int8  hhColourComponentBitCount; // Number of bits per colour component (16, 32,or 64);
	unsigned __int8	 hhChannelCount;			// Number of channels in file (3 = RGB, 1 = monochrome and so on).	
	unsigned __int32 hhWidth;					// Image width in pixels.
	unsigned __int32 hhHeight;				    // Image height in pixels.
} HDRHEADER;
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
	unsigned short getWidth() const;
	/*Returns the height of the image in pixels.*/
	unsigned short getHeight() const;
	/*Saves the image in whatever format it currently has.*/
	virtual void saveToFile(std::string fileName) = 0;
	/*Load the data from a istream.*/
	virtual void loadFromStream(std::istream& stream) = 0;
	
	imageDataFormat getImageDataFormat();		
	static const int HDRmagic = 0x7675;
	static const int BMP_Magic = 0x4d42;
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
	PAPImage* createImageFromImage(PAPImage& src, const imageDataFormat targetIDF);
	PAPImage* loadFromFile(std::string fileName);
	PAPImage* loadFromStream(std::istream& stream);
};

