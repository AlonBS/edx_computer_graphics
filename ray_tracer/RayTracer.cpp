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


Image* RayTracer::rayTrace(Camera & camera, Scene & scene, GLuint width, GLuint height)
{
	Image *image = new Image(width, height);

	vec3 color;


	// Render loop
	for (int i = 0 ; i < height ; ++i) {
		for (int j = 0 ; j < width ; ++j) {

			Ray ray = camera.generateRay(i + .5, j + .5);
			Intersection hit = intersectScene(scene, ray);

			if (hit.isValid) {
				color = computeLight(scene, hit);
				image->setPixel(i, j, color);
			}
		}
	}

	return image;
}


Intersection RayTracer::intersectScene(Scene & scene, Ray& ray)
{
	GLfloat minDist = FLT_MAX;
	GLfloat dist;
	vec3 point;
	vec3 normal;
	vec3 color;

	Intersection hit;

	for (Object *object : scene.getObjects()) {

		if (object->intersectsRay(ray, dist, point, normal, color)) {

			if (dist < minDist) {

//				std::cout << ":(" << color.x << "," << color.y << "," << color.z << ")" << std::endl;
//				std::cout << ":(" << normal.x << "," << normal.y << "," << normal.z << ")" << std::endl;
//				std::cout << "DIST:(" << dist << std::endl;

				minDist = dist;
				hit.point = point;
				hit.normal = normal;
				hit.color = color;
				hit.isValid = true;
			}
		}
	}

	if (minDist == FLT_MAX) {
		hit.isValid = false;
	}

	return hit;
}


vec3 RayTracer::computeLight(Scene& scene, Intersection& hit)
{

	return vec3(1.0f);
}
