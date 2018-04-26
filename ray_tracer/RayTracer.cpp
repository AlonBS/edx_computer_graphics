/*
 * RayTrace.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#include "RayTracer.h"
#include <future>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include "General.h"

using namespace std;



RayTracer::RayTracer() {
	// TODO Auto-generated constructor stub

}

RayTracer::~RayTracer() {
	// TODO Auto-generated destructor stub
}


Image* RayTracer::rayTrace(string& fileName, Camera & camera, Scene & scene, GLuint width, GLuint height, GLuint maxDepth)
{
	Image *image = new Image(width, height);

	size_t max = width * height;
	size_t cores = std::thread::hardware_concurrency();
	volatile atomic<size_t> count(0);
	vector<future<void>> future_vector;

	while (cores--)
	{
	    future_vector.push_back(
	        std::async(launch::async, [=, &camera, &scene, &count]()
	        {
	            while (true)
	            {
	                std::size_t index = count++;
	                if (index >= max)
	                    break;

	                GLuint i = index % width;
	                GLuint j = index / width;

	                Ray ray = camera.generateRay(i + .5, j - .5);
	                vec3 color = recursiveRayTrace(scene, ray, maxDepth);
	                image->setPixel(i, j, color);
	            }
	        }));
	}

	// To display progress bar, it's sufficient to 'actively' wait for one of the working threads
	std::future_status status;
	std::future<void>& f = future_vector[0];
	do {
		status = f.wait_for(std::chrono::milliseconds(100));
		if (status == std::future_status::timeout) {

			std::cout << "\tProgress: [ "<< setprecision(1) << fixed << (count / (GLfloat)max) * 100.0 << "% ] \r";
			std::cout.flush();
		}
	} while (status != std::future_status::ready);


	for (auto& e : future_vector) {
		e.get();
	}

	return image;
}


// OLD CODE - single threaded - for benchmark purposes
//
//Image* RayTracer::rayTrace(string& fileName, Camera & camera, Scene & scene, GLuint width, GLuint height, GLuint maxDepth)
//{
//	Image *image = new Image(width, height);
//	vec3 color;
//	GLfloat completed;
//
//	for (GLuint i = 0 ; i < width ; ++i)
//	{
//		for (GLuint j = 0 ; j < height; ++j)
//		{
//
//			Ray ray = camera.generateRay(i + .5, j - .5);
//			color = recursiveRayTrace(scene, ray, maxDepth);
//			image->setPixel(i, j, color);
//		}
//	}
//
//	return image;
//}


vec3 RayTracer::recursiveRayTrace(Scene& scene, Ray & ray, GLuint depth)
{
	vec3 color = COLOR_BLACK;

	if (depth == 0) {
		return COLOR_BLACK;
	}

	Intersection hit = intersectScene(scene, ray);
	if (!hit.isValid) {
		return COLOR_BLACK;
	}

	color = computeLight(scene, ray, hit);

	vec3 reflectedRayOrigin = hit.point;
	vec3 reflectedRayDir = glm::reflect(ray.direction, hit.normal);
	reflectedRayOrigin = reflectedRayOrigin + EPSILON * reflectedRayDir;
	Ray reflectedRay(reflectedRayOrigin , reflectedRayDir);

	return color + hit.properties._specular * recursiveRayTrace(scene, reflectedRay, --depth);

}


Intersection RayTracer::intersectScene(Scene & scene, Ray& ray)
{
	GLfloat minDist = INFINITE;
	GLfloat dist;
	vec3 point;
	vec3 normal;
	ObjectTexColors texColors;
	ObjectProperties objProps;

	Intersection hit;

	for (Object *object : scene.getObjects()) {

		if (object->intersectsRay(ray, dist, &point, &normal, &texColors, &objProps)) {

			if (dist < minDist) {

				minDist = dist;
				hit.point = point;
				hit.normal = normal;
				hit.texColors = texColors;

				hit.properties = objProps;
				hit.isValid = true;
			}
		}
	}

	if (minDist == FLT_MAX) {
		hit.isValid = false;
	}

	return hit;
}


vec3 RayTracer::computeLight(Scene& scene, Ray& r, Intersection& hit)
{
	vec3 color = COLOR_BLACK, tempColor;

	Ray shadowRay;
	vec3 srOrigin;
	vec3 srDir;
	GLfloat maxDist;

	vec3 eyeDir;
	vec3 halfAng;

	// The 'eye' direction is where the current ray was shot from, and hit.
	eyeDir = normalize(r.origin - hit.point);


	// Add point lights
	for (PointLight* p : scene.getPointLights()) {

		srDir = normalize(p->_position - hit.point);
		srOrigin = hit.point + EPSILON * srDir; // Move a little to avoid floating point errors
		shadowRay = Ray(srOrigin, srDir);
		maxDist = length(p->_position - hit.point);

		if (isVisibleToLight(scene.getObjects(), shadowRay, maxDist)) {

			halfAng = normalize(srDir + eyeDir);

			tempColor = __blinn_phong(hit.properties, hit.texColors, p->_color, srDir, hit.normal, halfAng);
			// take attenuation into account
			GLfloat atten = 1 / (scene.Attenuation().constant + scene.Attenuation().linear * maxDist + scene.Attenuation().quadratic * maxDist * maxDist);
			tempColor *= atten;
			color += tempColor;
		}
	}


	for (DirectionalLight* p : scene.getDirectionalLights()) {

		srDir = normalize(p->_direction);
		srOrigin = hit.point + EPSILON * srDir; // Move a little to avoid floating point errors
		shadowRay = Ray(srOrigin, srDir);
		maxDist = INFINITE;


		if (isVisibleToLight(scene.getObjects(), shadowRay, maxDist)) {

			halfAng = normalize(srDir + eyeDir);
			tempColor = __blinn_phong(hit.properties, hit.texColors, p->_color, srDir, hit.normal, halfAng);

			color += tempColor;
		}
	}


	//Ambient & Emission - regardless of lights
	color += hit.properties._ambient * hit.texColors._ambientTexColor +
			 hit.properties._emission * hit.texColors._ambientTexColor; // TODO Currently - ambient texture == emission texture
	return color;
}


bool RayTracer::isVisibleToLight(vector<Object*>& objects, Ray& shadowRay, GLfloat limit)
{
	GLfloat dist;
	vec3 point, normal;
	for (Object * o : objects) {

		if (o->intersectsRay(shadowRay, dist, nullptr, nullptr, nullptr, nullptr)) {

			// If there's a intersection to a object which is within limit (no 'after' the light)
			// then there's no visibility
			if (dist < limit) {
				return false;
			}
		}

	}

	return true;
}

vec3 RayTracer::__blinn_phong(const ObjectProperties& objProps,
							  const ObjectTexColors& objTexColors,
							  const vec3& lightColor,
							  const vec3& lightDir,
							  const vec3& normal,
							  const vec3& halfAng)
{

	// diffuse
	GLfloat diff = glm::max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * objProps._diffuse * objTexColors._diffuseTexColor;

	// Specular
	GLfloat spec = glm::pow(glm::max(dot(halfAng, normal), 0.0f), objProps._shininess);
	vec3 specular = spec * objProps._specular * objTexColors._specularTexColor;

	return (diffuse + specular) * lightColor;
}
