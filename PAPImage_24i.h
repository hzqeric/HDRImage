#pragma once
#include "PAPImage.h"

# pragma pack (1)
typedef struct tagPixel24i {
	unsigned __int8 rgbBlue;
	unsigned __int8 rgbGreen;
	unsigned __int8 rgbRed;	
} Pixel24i;
# pragma pack ()



class PAPImage_24i :
	public PAPImage
{
public:
	PAPImage_24i(const unsigned short width, const unsigned short height);
	virtual ~PAPImage_24i();
	void setDimensions(const unsigned short width, const unsigned short height) override;
	void saveToFile(std::string fileName) override;
	void loadFromStream(std::istream& stream) override;
	PAPImage* convertToIDF(imageDataFormat IDF) override;

	float getPixel_Red_16f(const unsigned short x, const unsigned short y) override;	
	float getPixel_Blue_16f(const unsigned short x, const unsigned short y) override;
	float getPixel_Green_16f(const unsigned short x, const unsigned short y) override;

	/* Setter for the red channel of the pixel(x,y). */
	void setPixel_Red_8i(const unsigned short x, const unsigned short y, const unsigned char red);
	/* Setter for the blue channel of the pixel(x,y). */
	void setPixel_Blue_8i(const unsigned short x, const unsigned short y, const unsigned char blue);
	/* Setter for the green channel of the pixel(x,y). */
	void setPixel_Green_8i(const unsigned short x, const unsigned short y, const unsigned char green);

	/* Access a unsinged char (native) for the red channel for the pixel(x,y)*/
	unsigned char getPixel_Red_8i(const unsigned short x, const unsigned short y);
	/* Access a unsinged char (native) for the blue channel for the pixel(x,y)*/
	unsigned char getPixel_Blue_8i(const unsigned short x, const unsigned short y);
	/* Access a unsinged char (native) for the green channel for the pixel(x,y)*/
	unsigned char getPixel_Green_8i(const unsigned short x, const unsigned short y);


protected:
	Pixel24i* _data;
	/*Takes into consideration the per row padding.*/
	unsigned int getIndex(unsigned short x, unsigned short y);
	unsigned int getDataSize();
};

