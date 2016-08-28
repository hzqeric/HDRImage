#pragma once
#include "PAPImage.h"

# pragma pack (1)
typedef struct tagPixel24bi {
	unsigned __int8 rgbBlue;
	unsigned __int8 rgbGreen;
	unsigned __int8 rgbRed;	
} Pixel24bi;
# pragma pack ()

typedef struct tagPixel32bi {
	unsigned __int8 rgbBlue;
	unsigned __int8 rgbGreen;
	unsigned __int8 rgbRed;
	unsigned __int8 rgbReserved;
} Pixel32bi;



class PAPImage_8i :
	public PAPImage
{
public:
	PAPImage_8i(const unsigned short width, const unsigned short height, const imageDataFormat IDF);
	virtual ~PAPImage_8i();
	void setDimensions(const unsigned short width, const unsigned short height) override;
	void saveToFile(std::string fileName) override;
	void loadFromStream(std::ifstream& stream) override;	

	/* Setter for the red channel of the pixel(x,y). */
	void setPixel_Red_8i(const unsigned short x, const unsigned short y, const unsigned char red);
	/* Setter for the blue channel of the pixel(x,y). */
	void setPixel_Blue_8i(const unsigned short x, const unsigned short y, const unsigned char blue);
	/* Setter for the green channel of the pixel(x,y). */
	void setPixel_Green_8i(const unsigned short x, const unsigned short y, const unsigned char green);

	/* Access a unsinged char (native) for the red channel for the pixel(x,y)*/
	unsigned char getPixel_Red_8i(const unsigned short x, const unsigned short y) const;
	/* Access a unsinged char (native) for the blue channel for the pixel(x,y)*/
	unsigned char getPixel_Blue_8i(const unsigned short x, const unsigned short y) const;
	/* Access a unsinged char (native) for the green channel for the pixel(x,y)*/
	unsigned char getPixel_Green_8i(const unsigned short x, const unsigned short y) const;


protected:
	unsigned __int8* _data;
	/*Takes into consideration the per row padding.*/
	unsigned int getIndex(unsigned short x, unsigned short y) const;
	unsigned int _dataSize;
	unsigned short _channelCount;
	unsigned int _rowSize;	
};

