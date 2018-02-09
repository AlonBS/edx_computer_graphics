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

private:

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	GLfloat shininess;

	mat4x4 transform;
	mat4x4 invTransform;	  // We compute it once, instead of each intersection test
	mat4x4 invTransposeTrans; // We compute it once, instead of each intersection test

protected:


public:


	Object();
	virtual ~Object();

	virtual bool intersectsRay(Ray &r, GLfloat &dist, vec3& normal, vec3& color) = 0;

	friend std::ostream& operator<< (std::ostream& out, const Object & obj);

	virtual const void print() const = 0;

	vec3& ambientVal () { return ambient; }
	vec3& diffuseVal () { return diffuse; }
	vec3& specularVal() { return specular; }
	vec3& emissionVal() { return emission; }
	GLfloat& shininessVal() { return shininess; }

	mat4x4& transformMat() { return transform; }
	mat4x4& invTransformMat() { return invTransform; }
	mat4x4& invTransposeTransMat() { return invTransposeTrans; }


protected:
	const GLfloat INFINITE = FLT_MAX;
	const GLfloat EPSILON = 0.000001f;
};




#endif /* OBJECT_H_ */
