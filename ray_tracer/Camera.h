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
using namespace std;

class Camera {


	vec3 eyeInit;
	vec3 center;
	vec3 upInit;

	vec3 u, v, w;

	GLfloat fovY, fovX;
	GLuint width, height;

public:

	Camera()
	:eyeInit(vec3(0,0,0)), center(vec3(0,0,0)), upInit(vec3(0,0,0)), fovY(0)
	,width(0), height(0), fovX(0)
	{}

	Camera(vec3& eyeInit, vec3& center, vec3& upInit, GLfloat fovY, GLuint width, GLuint height)
	:eyeInit(eyeInit), center(center), upInit(upInit), fovY(fovY)
	,width(width), height(height)
	{
		w = glm::normalize(eyeInit - center);
		u = glm::normalize(glm::cross(upInit, w));
		v = glm::cross(w,u);

		fovX = fovY; // TODO -  Change
	}

	Ray generateRay(GLfloat i, GLfloat j) {

		GLfloat hw = width / 2;
		GLfloat hh = height / 2;

		GLfloat alpha = tan(fovX / 2 ) * ( (j - hw) / hw );
		GLfloat beta  = tan(fovY / 2 ) * ( (hh - i) / i );

		vec3 origin = eyeInit;
		vec3 direction = alpha * u + beta * v - w;
		return Ray(origin, direction);
	}


	const void print () const
	{
		cout << "**** Camera Info ****" << std::endl;
		cout << "\tEye init: (" << eyeInit.x << "," << eyeInit.y << "," << eyeInit.z << ")" << endl;
		cout << "\tCenter: (" << center.x << "," << center.y << "," << center.z << ")" << endl;
		cout << "\tUp init: (" << upInit.x << "," << upInit.y << "," << upInit.z << ")" << endl;
		cout << "\tFov X: " << fovX << ". Fov Y: " << fovY << endl;
		cout << "\t(w,h) = (" << width << "," << height << ")" << endl;
		std::cout << "**** Camera Info ****" << std::endl;
	}



};


#endif /* CAMERA_H_ */
