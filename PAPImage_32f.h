#pragma once
#include "PAPImage.h"

typedef struct tagPixel16f {
	float rgbBlue;
	float rgbGreen;
	float rgbRed;
} Pixel16f;

class PAPImage_32f :
	public PAPImage
{
public:
	PAPImage_32f(const unsigned short width, const unsigned short height);
	~PAPImage_32f() override;		
	
	void setDimensions(const unsigned short width, const unsigned short height) override;	
	void saveToFile(std::string fileName) override;	
	void loadFromStream(std::istream& stream) override;

	float getPixel_Red_16f(const unsigned short x, const unsigned short y) const;
	float getPixel_Blue_16f(const unsigned short x, const unsigned short y) const;
	float getPixel_Green_16f(const unsigned short x, const unsigned short y) const;

	void setPixel_Red_16f(const unsigned short x, const unsigned short y, float red);
	void setPixel_Blue_16f(const unsigned short x, const unsigned short y, float blue);
	void setPixel_Green_16f(const unsigned short x, const unsigned short y, float green);
	void setPixel_16f(const unsigned short x, const unsigned short y, Pixel16f pixel);
	
protected:
	Pixel16f* _data;
	unsigned int getIndex(unsigned short x, unsigned short y) const;
	unsigned int getDataSize() const;
};

