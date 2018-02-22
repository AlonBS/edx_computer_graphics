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


Image* RayTracer::rayTrace(Camera & camera, Scene & scene, GLuint width, GLuint height, GLuint maxDepth)
{
	Image *image = new Image(width, height);
	vec3 color;

	// Render loop
	for (GLuint i = 0 ; i < height ; ++i) {
		for (GLuint j = 0 ; j < width ; ++j) {

			Ray ray = camera.generateRay(i + .5, j + .5);
			color = recursiveRayTrace(scene, ray, maxDepth);
			image->setPixel(i, j, color);
		}
	}

	return image;
}


vec3 RayTracer::recursiveRayTrace(Scene& scene, Ray& ray, GLuint depth)
{
	vec3 color = vec3(0.0f, 0.0f, 0.0f);
	ColorComponents co = {};

	if (depth == 0) {

		return vec3(0.0f, 0.0f, 0.0f);
	}


	Intersection hit = intersectScene(scene, ray);
	if (!hit.isValid) {
		return color;
	}

	co = computeLight(scene, ray, hit);

	vec3 reflectedRayOrigin = hit.point;
	vec3 reflectedRayDir = glm::reflect(ray.direction, hit.normal);
	reflectedRayOrigin = reflectedRayOrigin + EPSILON * reflectedRayDir;
	Ray reflectedRay(reflectedRayOrigin , reflectedRayDir);

	color = co.ambient + co.emission + co.diffuse + co.specular;
	return color + co.specular * recursiveRayTrace(scene, reflectedRay, --depth);

}


Intersection RayTracer::intersectScene(Scene & scene, Ray& ray)
{
	GLfloat minDist = FLT_MAX;
	GLfloat dist;
	vec3 point;
	vec3 normal;

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


ColorComponents RayTracer::computeLight(Scene& scene, Ray& r, Intersection& hit)
{
	ColorComponents co = {};
	vec3 diffuse = vec3(0.0f, 0.0f, 0.0f);
	vec3 specular = vec3(0.0f, 0.0f, 0.0f);

	Ray shadowRay;
	vec3 srOrigin;
	vec3 srDir;
	GLfloat maxDist;

	vec3 eyeDir;
	vec3 halfAng;



	// The 'eye' direction is where the current ray was shot from, and hit.
	eyeDir = normalize(r.origin - hit.point);

	//Ambient & Emission - regardless of lights
	co.ambient = hit.object->ambient();
	co.emission = hit.object->emission();

	// Add point lights
	for (PointLight* p : scene.getPointLights()) {


		srDir = normalize(p->_position - hit.point);
		srOrigin = hit.point + EPSILON * srDir; // Move a little to avoid floating point errors
		shadowRay = Ray(srOrigin, srDir);
		maxDist = length(p->_position - hit.point);

		if (isVisibleToLight(scene.getObjects(), shadowRay, maxDist)) {

			halfAng = normalize(srDir + eyeDir);
			__blinn_phong(hit.object, p->_color, srDir, hit.normal, halfAng, diffuse, specular);

			diffuse  /= (scene.Attenuation().constant + scene.Attenuation().linear * maxDist + scene.Attenuation().quadratic * maxDist * maxDist);
			specular /= (scene.Attenuation().constant + scene.Attenuation().linear * maxDist + scene.Attenuation().quadratic * maxDist * maxDist);

			co.diffuse += diffuse;
			co.specular += specular;
			//color = (color) / (scene.Attenuation().constant + scene.Attenuation().linear * maxDist + scene.Attenuation().quadratic * maxDist * maxDist);
		}
	}


	// take attenuation into account

	for (DirectionalLight* p : scene.getDirectionalLights()) {

		srDir = normalize(-p->_direction);
		srOrigin = hit.point + EPSILON * srDir; // Move a little to avoid floating point errors
		shadowRay = Ray(srOrigin, srDir);
		maxDist = INFINITE;

		if (isVisibleToLight(scene.getObjects(), shadowRay, maxDist)) {

			halfAng = normalize(srDir + eyeDir);
			__blinn_phong(hit.object, p->_color, srDir, hit.normal, halfAng, diffuse, specular);

			co.diffuse += diffuse;
			co.specular += specular;
		}
	}

	return co;
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

void RayTracer::__blinn_phong(Object* obj, vec3& lightColor, vec3& lightDir, vec3& normal, vec3& halfAng, vec3& diffuse, vec3& specular)
{
	vec3 result = vec3(0.0, 0.0, 0.0);

	// diffuse
	GLfloat diff = glm::max(dot(normal, lightDir), 0.0f);
	diffuse = diff * obj->diffuse();
	diffuse *= lightColor;

	// Specular
	GLfloat spec = glm::pow(glm::max(dot(halfAng, normal), 0.0f), obj->shininess());
	specular = spec * obj->specular();
	specular *= lightColor;

}
