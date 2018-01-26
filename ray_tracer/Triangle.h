/*
 * Triangle.h
 *
 *  Created on: Jan 26, 2018
 *      Author: alonbs
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "Object.h"

#include <glm/glm.hpp>


class Triangle: public Object {

private:

		vec3 A,B,C ; // This triangle vertices
		vec3 N;     // Face normal to the triangle


public:

	Triangle(vec3& va, vec3& vb, vec3& vc);

	virtual ~Triangle();

	virtual bool intersectsRay(Ray &r, GLfloat &dist, glm::vec3& normal);

	bool intersectsRay2(Ray &r, GLfloat &dist, glm::vec3& normal);

	virtual const void print() const {};
};

#endif /* TRIANGLE_H_ */
