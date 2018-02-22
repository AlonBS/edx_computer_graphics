/*
 * RayTrace.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include "GL/glew.h"
#include "Image.h"
#include "Scene.h"
#include "Camera.h"
#include "Ray.h"

#include <vector>


typedef struct Intersection {

	bool isValid;

	vec3 point;
	vec3 normal;

	Object *object; // The object the ray intersected

}Intersection;

typedef struct ColorComponents {

	vec3 ambient;
	vec3 emission;
	vec3 diffuse;
	vec3 specular;

}ColorComponents;


class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();


	Image* rayTrace(string& fileName, Camera & camera, Scene & scene, GLuint width, GLuint height, GLuint maxDepth);

private:

	vec3 recursiveRayTrace(Scene& scene, Ray& ray, GLuint depth);

	Intersection intersectScene(Scene & scene, Ray& ray);

	ColorComponents computeLight(Scene & scene, Ray& r, Intersection& hit);

	bool isVisibleToLight(vector<Object*>& objects, Ray& shadowRay, GLfloat limit);


	void __blinn_phong(Object* obj, vec3& lightColor, vec3& lightDir, vec3& normal, vec3& halfAng, vec3& diffuse, vec3& specular);

};



#endif /* RAYTRACER_H_ */
