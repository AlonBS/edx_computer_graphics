/*
 * Object.h
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Ray.h"
#include <cstdint>
#include <GL/glew.h>

using namespace glm;

class Object {
public:


	Object() {}
	virtual ~Object() {}

	virtual bool intersectsRay(Ray &r, GLfloat &dist, vec3& normal) = 0;

	friend std::ostream& operator<< (std::ostream& out, const Object & obj);

	virtual const void print() const = 0;


protected:
	const GLfloat INFINITE = FLT_MAX;
	const GLfloat EPSILON = 0.000001f;
};




#endif /* OBJECT_H_ */
