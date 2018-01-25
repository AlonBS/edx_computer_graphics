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

	Pixel **image;

public:

	Image (int height, int width)
	:height(height), width(width)
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



	int getWidth() { return this->width; }
	int getHeight() { return this->height; }
	BYTE* toByteArray() { return (BYTE*)&image[0][0]; }

	inline friend std::ostream& operator<< (std::ostream &out, const Image &image);

	inline void tt()
	{
		Pixel p;
		for (int i = 0 ; i < this->height ; ++i) {
			for (int j = 0 ; j < this->width ; ++j) {

				p = this->image[i][j];
				int r = p.r;
				int g = p.g;
				int b = p.b;

				std::cout << "(" << r << "," << g << "," << b << ")";
			}
			std::cout << std::endl;
		}
	}

};

inline std::ostream& operator<< (std::ostream &out, const Image &image)
{
	for (int i = 0 ; i < image.height ; ++i) {
		for (int j = 0 ; j < image.width ; ++j) {
			//out << image.image[i][j].r;
		}
		out << std::endl;
	}

	return out;
}





#endif /* IMAGE_H_ */
