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


Image* RayTracer::rayTrace(string& fileName, Camera & camera, Scene & scene, GLuint width, GLuint height, GLuint maxDepth)
{
	Image *image = new Image(width, height);
	vec3 color;
	GLfloat completed;

	// Render loop
	{
//#pragma omp parallel for collapse(2)
		for (GLuint i = 0 ; i < width ; ++i)
		{
			for (GLuint j = 0 ; j < height; ++j)
			{
				Ray ray = camera.generateRay(i + .5, j - .5);
				color = recursiveRayTrace(scene, ray, maxDepth);
				image->setPixel(i, j, color);
			}
		}
	}

	return image;
}


vec3 RayTracer::recursiveRayTrace(Scene& scene, Ray & ray, GLuint depth)
{
	vec3 color = vec3(0.0f, 0.0f, 0.0f);

	if (depth == 0) {
		return vec3(0.0f, 0.0f, 0.0f);
	}

	Intersection hit = intersectScene(scene, ray);
	if (!hit.isValid) {
		return vec3(0.0f, 0.0f, 0.0f);
	}

	color = computeLight(scene, ray, hit);

	vec3 reflectedRayOrigin = hit.point;
	vec3 reflectedRayDir = glm::reflect(ray.direction, hit.normal);
	reflectedRayOrigin = reflectedRayOrigin + EPSILON * reflectedRayDir;
	Ray reflectedRay(reflectedRayOrigin , reflectedRayDir);

	return color + hit.object->specular() * recursiveRayTrace(scene, reflectedRay, --depth);

}


Intersection RayTracer::intersectScene(Scene & scene, Ray& ray)
{
	GLfloat minDist = FLT_MAX;
	GLfloat dist;
	vec3 point;
	vec3 normal;
	vec3 texColor;

	Intersection hit;

	for (Object *object : scene.getObjects()) {

		if (object->intersectsRay(ray, dist, point, normal)) {

			if (dist < minDist) {

				minDist = dist;
				hit.point = point;
				hit.normal = normal;
				hit.texColor = texColor;

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


vec3 RayTracer::computeLight(Scene& scene, Ray& r, Intersection& hit)
{
	vec3 color = vec3(0.0f, 0.0f, 0.0f), tempColor;

	Ray shadowRay;
	vec3 srOrigin;
	vec3 srDir;
	GLfloat maxDist;

	vec3 eyeDir;
	vec3 halfAng;

	vec3 diffuseTexture = hit.object->getTextureColor(hit.point);


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

			tempColor = __blinn_phong(hit.object, p->_color, srDir, hit.normal, halfAng);
			tempColor *= diffuseTexture;
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
			tempColor = __blinn_phong(hit.object, p->_color, srDir, hit.normal, halfAng);
			tempColor *= diffuseTexture;

			color += tempColor;
		}
	}

	//Ambient & Emission - regardless of lights
	color += hit.object->ambient() * diffuseTexture + hit.object->emission() * diffuseTexture;

	return color;
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

vec3 RayTracer::__blinn_phong(Object* obj, vec3& lightColor, vec3& lightDir, vec3& normal, vec3& halfAng)
{
	// diffuse
	GLfloat diff = glm::max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = diff * obj->diffuse();
	diffuse *= lightColor;

	// Specular
	GLfloat spec = glm::pow(glm::max(dot(halfAng, normal), 0.0f), obj->shininess());
	vec3 specular = spec * obj->specular();

	return (diffuse + specular) * lightColor;
}
