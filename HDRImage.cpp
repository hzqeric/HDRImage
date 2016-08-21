// HDRImage.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include "PAPImage.h"
#include "PAPImage_32i.h"
#include "PAPImage_24i.h"

using namespace std;

int main()
{	
	cout << "Creating test image: testFile.bmp.\n";
	PAPImageFactory *factory = new PAPImageFactory();
	PAPImage_32i* img32 = (PAPImage_32i*) factory->createImage(63, 64, IDF32i);

	for (int i = 10; i < 54; i++) {
		img32->setPixel_Blue_8i(i, 10, 255);
		img32->setPixel_Green_8i(10, i, 255);
	}
	
	for (int x = 0; x < img32->getWidth() -1; x++) {
		img32->setPixel_Red_8i(x, 0, 255);
		img32->setPixel_Red_8i(x, img32->getHeight() -1, 255);	
	}

	cout << "Saving result to file.\n";
	img32->saveToFile("testFile.bmp");
	
	cout << "Deleteing object.\n";
	delete img32;

	cout << "Testing to load from file: testFile2.bmp.\n";
	PAPImage* img = factory->loadFromFile("testfile2.bmp");

	cout << "image loaged. saving result to new file: testfile3.bmp.\n";		
	img->saveToFile("testFile3.bmp");
	delete img; 
	
	cout << "Attempting to convert 24bit to 32bit. \n";
	PAPImage* img24 = factory->loadFromFile("testfile24.bmp");

	cout << "  24bit image loaded.\n";
	img = img24->convertToIDF(IDF32i);
	cout << "  Saving 32bit iamge.\n";
	img->saveToFile("testfile32.bmp");

	delete factory;
    return 0;
}

