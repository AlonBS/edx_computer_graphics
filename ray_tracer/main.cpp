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

	string fileName = "./scenes/scene1.test";
	RenderInfo renderInfo;
	readFile(fileName.c_str(), renderInfo);

	renderInfo.camera.print();

	RayTracer rayTracer;
	Image *img = rayTracer.rayTrace(renderInfo.camera, renderInfo.scene, renderInfo.height, renderInfo.width);

	//img->saveImage(renderInfo.outputFile);
	string output = "./scenes/scene1.test_result";
	img->saveImage(output);
	delete img;

	cout << "Done " << endl;
	return 0;
}



