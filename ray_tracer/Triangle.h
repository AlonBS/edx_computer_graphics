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

		vec3 A,B,C ; 	// This triangle vertices
		vec3 AN,BN,CN ; // Normals to the above vertices, respectively.
		vec3 N;     	// Face normal to the triangle

		vec3 color;

		bool __iRay(Ray &r, GLfloat &dist, glm::vec3& normal);
		bool __iRay2(Ray &r, GLfloat &dist, glm::vec3& normal);

public:

	Triangle(vec3& va, vec3& vb, vec3& vc);
	Triangle(vec3& va, vec3& vb, vec3& vc, vec3& vaNorm, vec3& vbNorm, vec3& vcNorm);

	virtual ~Triangle();

	virtual bool intersectsRay(Ray &r, GLfloat &dist, glm::vec3& normal);


	virtual const void print() const {};
};

#endif /* TRIANGLE_H_ */
