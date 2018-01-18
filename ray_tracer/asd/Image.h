/*
 * Image.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef IMAGE_H_
#define IMAGE_H_


#include <FreeImage.h>
#include <cassert>

class Pixel {

	BYTE r;
	BYTE g;
	BYTE b;

	Pixel(BYTE r, BYTE g, BYTE b) : r(r), g(g), b(b) {}

};

class Image {
private:

	int width;
	int height;

	Pixel **image;


	Image (int width, int height)
	:width(width), height(height)
	{
		image = new Pixel*[height];
		for (int i = 0 ; i < height ; ++i) {
			image[i] = new Pixel[width];
		}
	}

	~Image() {
		for (int i = 0 ; i < height; ++i) {
			delete [] image[i];
		}

		delete [] image;
	}

	Pixel& operator()(int row, int col) {
		assert(row >= 0 && row < height);
		assert(col >= 0 && col < width);

		return image[row][col];
	}
};





#endif /* IMAGE_H_ */
