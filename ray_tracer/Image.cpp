/*
 * Image.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */


#include <cstdio>
#include <iostream>
#include <cassert>
#include "Image.h"
#include "General.h"
#include "glm/glm.hpp"

using namespace std;

#define RGBSIZE 24

int Image::FreeImage_objs_cnt = 0;

glm::vec3 Image::clamp (glm::vec3& v)
{
	glm::vec3 clampedV = v;

	(clampedV.r > 1.0f) ? clampedV.r = 1.0f : (clampedV.r < 0.0f) ? clampedV.r = 0.0f : clampedV.r;
	(clampedV.g > 1.0f) ? clampedV.g = 1.0f : (clampedV.g < 0.0f) ? clampedV.g = 0.0f : clampedV.g;
	(clampedV.b > 1.0f) ? clampedV.b = 1.0f : (clampedV.b < 0.0f) ? clampedV.b = 0.0f : clampedV.b;

	return clampedV;
}




Image::Image(int width, int height)
:width(width), height(height)
{
	if (FreeImage_objs_cnt == 0) {
		// First Image
		FreeImage_Initialise();
	}

	this->bitmap = FreeImage_Allocate(width, height, RGBSIZE);
	++FreeImage_objs_cnt;
}


Image::~Image() {

	--FreeImage_objs_cnt;
	FreeImage_Unload(this->bitmap);
	if (FreeImage_objs_cnt == 0) {
		// Last image
		FreeImage_DeInitialise();
	}
}

void Image::setPixel(int row, int col, glm::vec3& rgbColors)
{
	assert(row >= 0 && row < height);
	assert(col >= 0 && col < width);

	RGBQUAD color;

	glm::vec3 clampedValue = this->clamp(rgbColors);
	color.rgbRed = clampedValue.r * 255;
	color.rgbGreen = clampedValue.g * 255;
	color.rgbBlue = clampedValue.b * 255;

	FreeImage_SetPixelColor(this->bitmap, col, row, &color);
}


const void Image::saveImage(std::string& fileName) const
{
	//FIF_PNG
	FreeImage_Save(FIF_JPEG, bitmap, fileName.c_str(), 0);
	std::cout << "Image saved to: " << fileName << std::endl;
}
