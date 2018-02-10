/*
 * RayTrace.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#include "RayTracer.h"
#include <iostream>
#include <vector>
#include "General.h"



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
				//color = hit.object->ambient();
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

		if (object->intersectsRay(ray, dist, point, normal)) {

			if (dist < minDist) {

				//				std::cout << ":(" << color.x << "," << color.y << "," << color.z << ")" << std::endl;
				//				std::cout << ":(" << normal.x << "," << normal.y << "," << normal.z << ")" << std::endl;
				//				std::cout << "DIST:(" << dist << std::endl;

				minDist = dist;
				hit.point = point;
				hit.normal = normal;
				hit.object = object;
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
	vec3 color = vec3(0.0f, 0.0f, 0.0f);
	Ray shadowRay = Ray();
	vec3 srOrigin;
	vec3 srDir;
	GLfloat maxDist;


	for (PointLight* p : scene.getPointLights()) {

		srDir = normalize(p->_position - hit.point);
		srOrigin = hit.point + EPSILON * srDir; // Move a little to avoid floating point errors
		shadowRay = Ray(srDir , srOrigin);

		maxDist = length(p->_position - hit.point);

		if (isVisibleToLight(scene.getObjects(), shadowRay, maxDist)) {

			// compute bling-phong lighting

			color += p->_color;
			//color += computeLight()
		}
	}

	for (DirectionalLight* p : scene.getDirectionalLights()) {

		srDir = -p->_direction;
		srOrigin = hit.point + EPSILON * srDir; // Move a little to avoid floating point errors
		shadowRay = Ray(srDir , srOrigin);
		maxDist = INFINITE;

		if (isVisibleToLight(scene.getObjects(), shadowRay, maxDist)) {

			// compute bling-phong lighting
			color += p->_color;
			//color += computeLight()
		}
	}

	return hit.object->ambient() + color;
}


bool RayTracer::isVisibleToLight(vector<Object*>& objects, Ray& shadowRay, GLfloat limit)
{
	GLfloat dist;
	vec3 point, normal;
	for (Object * o : objects) {

		if (o->intersectsRay(shadowRay, dist, point, normal)) {

			// If there's a intersection to a object which is within limit (no 'after' the light)
			// then there's no visibility
			if (dist < limit) {
				return false;
			}
		}

	}

	return true;

}
