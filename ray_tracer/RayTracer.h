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

class RayTracer {
public:
	RayTracer();
	virtual ~RayTracer();


	Image* rayTrace(Camera & camera, Scene & scene, GLuint width, GLuint height);

private:

	bool intersectScene(Scene & scene, Ray& ray);

};



#endif /* RAYTRACER_H_ */
