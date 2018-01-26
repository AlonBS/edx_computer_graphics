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
//
//int main()
//{
//	cout << "Ray Tracer working..." << endl;
//
//	string fileName = "./scenes/scene0a.test";
//	RenderInfo renderInfo;
//	readFile(fileName.c_str(), renderInfo);
//
//	renderInfo.camera.print();
//
//	RayTracer rayTracer;
//	Image *img = rayTracer.rayTrace(renderInfo.camera, renderInfo.scene, renderInfo.height, renderInfo.width);
//
//	img->saveImage(renderInfo.outputFile);
//	delete img;
//
//	cout << "Done " << endl;
//	return 0;
//}

#include "Triangle.h"

int main()
{

	vec3 a = vec3(-2.0f, 0.0, 0.0f); vec3 b = vec3(2.0f, 0.0, 0.0f); vec3 c = vec3(0.0f, 0.0, 2.0f);

	vec3 o1 = vec3(0.0f, -2.0f, 0.5f); vec3 d1 = vec3(0.0f, 1.0f, 0.0f);
	//vec3 o2 = vec3(0.0f, -1.0f, 0.0f); vec3 d1 = vec3(0.0f, -1.0f, 0.0f);


	Ray r(o1, d1);
	Triangle t1(a, b, c);

	GLfloat dist1, dist2;
	vec3    N1, N2;

	if (t1.intersectsRay (r, dist1, N1) ) {
		cout << "DIST: " << dist1 << endl;
		cout << "NORMAL: (" << N1.x << "," << N1.y << "," << N1.z << ")" << endl;
	}

	if (t1.intersectsRay2(r, dist2, N2)) {
		cout << "DIST2: " << dist2 << endl;
		cout << "NORMAL2: (" << N2.x << "," << N2.y << "," << N2.z << ")" << endl;
	}

}


