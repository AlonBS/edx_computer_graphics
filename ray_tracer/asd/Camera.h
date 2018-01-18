/*
 * Camera.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include "Ray.h"

using namespace glm;

class Camera {


	vec3 eyeInit;
	vec3 center;
	vec3 upInit;

	vec3 u, v, w;

	GLfloat fovY, fovX;
	GLuint width, height;


	Camera(vec3 eyeInit, vec3 center, vec3 upInit, GLfloat fovY)
	:eyeInit(eyeInit), center(center), upInit(upInit), fovY(fovY)
	,width(width), height(height)
	{
		w = glm::normalize(eyeInit - center);
		u = glm::normalize(glm::cross(upInit, w));
		v = glm::cross(w,u);

		fovX = fovY; // Change
	}

	Ray generateRay(GLuint i, GLuint j) {

		float hw = width / 2;
		float hh = height / 2;

		float alpha = tan(fovX / 2 ) * ( (j - hw) / hw );
		float beta  = tan(fovY / 2 ) * ( (hh - i) / i );

		vec3 origin = eyeInit;
		vec3 direction = alpha * u + beta * v - w;
		return Ray(origin, direction);
	}



};


#endif /* CAMERA_H_ */
