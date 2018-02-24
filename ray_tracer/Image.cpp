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
	this->bitmap = FreeImage_Allocate(width, height, RGBSIZE);
}

Image::Image(std::string& fileName)
{

	this->bitmap = FreeImage_Load(FIF_PNG, fileName.c_str());

	this->height = FreeImage_GetHeight(this->bitmap);
	this->width = FreeImage_GetWidth(this->bitmap);

}


Image::~Image() {

	FreeImage_Unload(this->bitmap);
}

void Image::setPixel(int col, int row, glm::vec3& rgbColors)
{
///	assert(row >= 0 && row < height);
///	assert(col >= 0 && col < width);

	RGBQUAD color;

	glm::vec3 clampedValue = this->clamp(rgbColors);
	color.rgbRed = clampedValue.r * 255;
	color.rgbGreen = clampedValue.g * 255;
	color.rgbBlue = clampedValue.b * 255;

	//FreeImage_SetPixelColor(this->bitmap, col, row, &color);
	FreeImage_SetPixelColor(this->bitmap, col, row, &color);
}

vec3 Image::getPixel(int col, int row)
{
	vec3 color;
	RGBQUAD pixel;

	FreeImage_GetPixelColor(this->bitmap, col, row, &pixel);
	color.r = pixel.rgbRed / 255.0f;
	color.g = pixel.rgbGreen / 255.0f;
	color.b = pixel.rgbBlue / 255.0f;

	return this->clamp(color);
}



const void Image::saveImage(std::string& fileName) const
{
	//FIF_PNG
	FreeImage_Save(FIF_PNG, bitmap, fileName.c_str(), 0);
	std::cout << "\tImage saved to: " << fileName << std::endl;
}
