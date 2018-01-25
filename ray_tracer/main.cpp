

#include <iostream>
#include <FreeImage.h>

#include "SceneParser.h"
#include "Camera.h"
#include "RenderInfo.h"
#include "Image.h"
#include "RayTracer.h"





using namespace std;


static void
saveScene(std::string& fileName, Image* image)
{
	BYTE *pixels = new BYTE[3*image->getHeight() * image->getWidth()];

	for (int i = 0 ; i < image->getHeight() ; ++ i) {
		for (int j = 0 ; j < image->getWidth() ; ++j) {

			pixels[(i * image->getWidth() + j) + 0] = (*image)(i,j).r;
			pixels[(i * image->getWidth() + j) + 1] = (*image)(i,j).g;
			pixels[(i * image->getWidth() + j) + 2] = (*image)(i,j).b;
		}
	}

	pixels = image->toByteArray();
	FIBITMAP *img = FreeImage_ConvertFromRawBits(pixels, image->getWidth(),
			                                             image->getHeight(),
														 image->getWidth() * 3,
														 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
	FreeImage_Save(FIF_PNG, img, fileName.c_str(), 0);
	//delete[] pixels;
}


#include "Object.h"
#include "Sphere.h"
#include "Ray.h"
#include <vector>


int main() 
{
	cout << "Ray Tracer working..." << endl;


	FreeImage_Initialise();

	RenderInfo renderInfo;

	string fileName = "./scenes/scene0a.test";
	readFile(fileName.c_str(), renderInfo);

	renderInfo.camera.print();

	RayTracer rayTracer;
	Image *img = rayTracer.rayTrace(renderInfo.camera, renderInfo.scene, renderInfo.height, renderInfo.width);

	for (Object *o : renderInfo.scene.getObjects()) {
		cout << *o ;
	}

	//cout << *img << endl;

	img->tt();

	string outfile = "./aaaaa";
	saveScene(outfile, img);

	FreeImage_DeInitialise();


	cout << "Done " << endl;

	return 0;
}



/*
 *
 *
	vec3 center = vec3(0.0f, 0.0f, 0.0f);
	GLfloat radius = 1;
	Sphere ss(center, radius);
	GLfloat dist;
	vec3 normal;

	vec3 tt = glm::normalize(glm::vec3(5.0, 3, 2));
	cout << "DOTTT: " << glm::dot(tt,tt) << endl;


	vec3 o1 = vec3(0, 0, -0.5); vec3 d1 = vec3(0, 0, 1);
	vec3 o2 = vec3(0, -2, 0); vec3 d2 = vec3(0, 1, 0);
	vec3 o3 = vec3(-2, 0, 0); vec3 d3 = vec3(1, 0, 0);

	vec3 o4 = vec3(0, 0, -0.5); vec3 d4 = vec3(0, 0, -1);
	vec3 o5 = vec3(0, -2, 0); vec3 d5 = vec3(0, -1, 0);
	vec3 o6 = vec3(-2, 0, 0); vec3 d6 = vec3(-1, 0, 0);


	Ray r1(o1, d1), r2(o2, d2), r3(o3, d3);
	Ray r4(o4, d4), r5(o5, d5), r6(o6, d6);


	std::vector<Ray> vecs;
	vecs.push_back(r1); vecs.push_back(r2); vecs.push_back(r3);
	vecs.push_back(r4); vecs.push_back(r5); vecs.push_back(r6);

	for (Ray r : vecs) {

		if (ss.intersectsRay(r, dist, normal) ) {

			cout << "Int. Dist: " << dist << " . Normal: " << normal.x << ", " << normal.y << " ," << normal.z << "." << std::endl;
		}
		else {
			std::cout << "No intersection!" << std::endl;
		}

	}
 */


