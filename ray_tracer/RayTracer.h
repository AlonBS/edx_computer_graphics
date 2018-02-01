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

typedef struct Intersection {

	bool isValid;

	vec3 normal;
	vec3 color;

}Intersection;


class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();


	Image* rayTrace(Camera & camera, Scene & scene, GLuint width, GLuint height);

private:

	Intersection intersectScene(Scene & scene, Ray& ray);

};



#endif /* RAYTRACER_H_ */
