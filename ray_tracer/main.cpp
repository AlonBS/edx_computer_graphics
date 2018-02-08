//
//
#include <iostream>
#include <FreeImage.h>
//
#include "SceneParser.h"
#include "Camera.h"
#include "RenderInfo.h"
#include "Image.h"
#include "RayTracer.h"


using namespace std;

int main()
{
	cout << "Ray Tracer working..." << endl;

	string fileName = "./scenes/scene0b.test";
	RenderInfo renderInfo = SceneParser::readFile(fileName.c_str());

	renderInfo.camera->print();

	RayTracer rayTracer;
	Image *img = rayTracer.rayTrace(*renderInfo.camera, renderInfo.scene, renderInfo.width, renderInfo.height);

	//img->saveImage(renderInfo.outputFile);
	string output = "./scenes/scene1.test_result";
	img->saveImage(output);
	delete img;
	delete renderInfo.camera;

	cout << "Done " << endl;
	return 0;
}

//
//#define WIDTH 200
//#define HEIGHT 300
//#define BPP 24
//
//using namespace std;
//
//int main()
//{
//	FreeImage_Initialise();
//	FIBITMAP *bitmap = FreeImage_Allocate(WIDTH, HEIGHT, BPP);
//	RGBQUAD color;
//
//	for (int i = 0 ; i < WIDTH ; ++i) {
//		for (int j = 0 ; j < HEIGHT ; ++j) {
//
//			color.rgbGreen = 255.0f;
//			FreeImage_SetPixelColor(bitmap, i, j, &color);
//		}
//	}
//
//	FreeImage_Save(FIF_PNG, bitmap, "test.png", 0);
//
//	FreeImage_DeInitialise();
//
//}
