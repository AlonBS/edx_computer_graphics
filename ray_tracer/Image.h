/*
 * Image.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef IMAGE_H_
#define IMAGE_H_

#include <cstdio>
#include <iostream>
#include <FreeImage.h>
#include <cassert>

#include <glm/glm.hpp>

class Pixel {

public:

	BYTE r;
	BYTE g;
	BYTE b;

	Pixel() : r(0), g(0), b(0) {}

	Pixel(BYTE r, BYTE g, BYTE b) : r(r), g(g), b(b) {}

	inline friend std::ostream& operator<< (std::ostream &out, const Pixel &pixel);

};

inline std::ostream& operator<< (std::ostream &out, const Pixel &pixel)
{
	out << "(" << pixel.r << "," << pixel.g << "," << pixel.b << ")";
	return out;
}


class Image {
private:


	int width;
	int height;

	FIBITMAP* bitmap;

	glm::vec3 clamp (glm::vec3& v);

public:


	Image (int height, int width);

	~Image();

	void setPixel(int row, int col, glm::vec3& rgbColors);

	int getWidth() { return this->width; }
	int getHeight() { return this->height; }


	const void saveImage(std::string& fileName) const;


};


#endif /* IMAGE_H_ */
