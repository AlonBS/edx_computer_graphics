

#include <iostream>
#include <FreeImage.h>

#include "SceneParser.h"
#include "Camera.h"
#include "RenderInfo.h"
#include "Image.h"



using namespace std;


static void
saveScene(std::string& fileName)
{
	int pix = 100 * 100;
	BYTE *pixels = new BYTE[3*pix];

	for (int i = 0 ; i < 3*pix ; i+=6) {

		pixels[i] = 0xff;
		pixels[i+1] = 0x00;
//		pixels[i+2] = 0x00;
	}


	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, 100, 100, 100 * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, fileName.c_str(), 0);
	delete[] pixels;
}


int main() 
{
	cout << "Hello Ray Tracer " << endl;


	FreeImage_Initialise();


	RenderInfo renderInfo;

	string fileName = "./aa";
	readFile(fileName.c_str(), renderInfo);

	TraceTray(renderInfo.camera, renderInfo.scene)



	//saveScene(fileName);

	FreeImage_DeInitialise();

	return 0;
}

