// HDRImage.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include "PAPImage.h"
#include "PAPImage_32f.h"
#include "PAPImage_8i.h"

using namespace std;

int main()
{	
	cout << "Creating test image: testFile.bmp.\n";
	PAPImageFactory *factory = new PAPImageFactory();	
	PAPImage_8i* img8 = (PAPImage_8i*) factory->createImageFromFile("testFile.bmp");	

	img8->setPixel_Green_8i(0, 0, 255);
	img8->setPixel_Green_8i(0, 1, 255);
	img8->setPixel_Green_8i(0, img8->getHeight() - 1, 255);

	img8->setPixel_Blue_8i(1, 0, 255);
	img8->setPixel_Blue_8i(1, 1, 255);
	img8->setPixel_Blue_8i(1, img8->getHeight() - 1, 255);

	img8->setPixel_Green_8i(4, 0, 255);
	img8->setPixel_Green_8i(4, 1, 255);
	img8->setPixel_Green_8i(4, img8->getHeight() - 1, 255);

	img8->setPixel_Blue_8i(5, 0, 255);
	img8->setPixel_Blue_8i(5, 1, 255);
	img8->setPixel_Blue_8i(5, img8->getHeight() - 1, 255);

	img8->setPixel_Red_8i(img8->getWidth() - 1, img8->getHeight() - 1, 255);
	img8->setPixel_Red_8i(img8->getWidth() - 1, 0, 255);

	cout << "Saving test image: testFile1_24.bmp.\n";
	img8->saveToFile("testFile1_24.bmp");	

	cout << "Createing test image" << endl;
	PAPImage_8i* img2 = (PAPImage_8i*) factory->createImage(336, 279, IDF8i3);

	img2->setPixel_Green_8i(0, 0, 255);
	img2->setPixel_Green_8i(0, 1, 255);
	img2->setPixel_Green_8i(0, img2->getHeight() - 1, 255);

	img2->setPixel_Blue_8i(1, 0, 255);
	img2->setPixel_Blue_8i(1, 1, 255);
	img2->setPixel_Blue_8i(1, img2->getHeight() - 1, 255);

	img2->setPixel_Green_8i(4, 0, 255);
	img2->setPixel_Green_8i(4, 1, 255);
	img2->setPixel_Green_8i(4, img2->getHeight() - 1, 255);

	img2->setPixel_Blue_8i(5, 0, 255);
	img2->setPixel_Blue_8i(5, 1, 255);
	img2->setPixel_Blue_8i(5, img2->getHeight() - 1, 255);

	img2->setPixel_Red_8i(img2->getWidth() - 1, img2->getHeight() - 1, 255);
	img2->setPixel_Red_8i(img2->getWidth() - 1, 0, 255);

	img2->saveToFile("TestImage_cust.bmp");	
	
	PAPImage_8i* img3 = (PAPImage_8i*) factory->createImageFromImage(*img2, IDF8i4);
	img3->saveToFile("test32.bmp");

	delete img3;
	delete img8;
	delete img2;
	
	delete factory;
    return 0;
}

