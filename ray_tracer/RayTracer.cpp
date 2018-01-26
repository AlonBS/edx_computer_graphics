/*
 * RayTrace.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#include "RayTracer.h"
#include <iostream>



RayTracer::RayTracer() {
	// TODO Auto-generated constructor stub

}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}


Image* RayTracer::rayTrace(Camera & camera, Scene & scene, GLuint height, GLuint width)
{
	Image *image = new Image(height, width);


	// Render loop
	for (int i = 0 ; i < height ; ++i) {
		for (int j = 0 ; j < width ; ++j) {

			Ray ray = camera.generateRay(i, j);

			//Intersection hit = Intersect(ray, scene);

			if (intersectScene(scene, ray)) {
				vec3 a = vec3(1.0f, 0.0f, 0.0f);
				image->setPixel(i, j, a);
			}
		}
	}

	return image;
}


bool RayTracer::intersectScene(Scene & scene, Ray& ray)
{
	GLfloat minDist = FLT_MAX;
	GLfloat dist;
	vec3 normal;

	for (Object *object : scene.getObjects()) {

		if (object->intersectsRay(ray, dist, normal)) {

			if (dist < minDist) {
				minDist = dist;
			}
		}
		else {
		}
	}

	if (minDist == FLT_MAX) {
		return FALSE;
	}

	return true;
}
