/*
 * RayTrace.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef RAYTRACE_H_
#define RAYTRACE_H_

#include "GL/glew.h"
#include "Image.h"
#include "Scene.h"
#include "Camera.h"
#include "Ray.h"

class RayTrace {
public:
	RayTrace();
	virtual ~RayTrace();


	Image* rayTrace(Camera & camera, Scene & scene, GLuint width, GLuint height);



};


Image* RayTrace::rayTrace(Camera & camera, Scene & scene, GLuint height, GLuint width)
{
	Image *image = new Image(width, height);

	// Render loop
	for (int i = 0 ; i < height ; ++i) {
		for (int j = 0 ; j < width ; ++j) {


			Ray ray = camera.generateRay(i, j);
			//Intersection hit = Intersect(ray, scene);
			//image(i,j) = FindColor(Pixel(0,0,0));
			(*image)(i,j) = Pixel(0,0,0);
		}
	}

	return image;
}

#endif /* RAYTRACE_H_ */
