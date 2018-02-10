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

#define EPSILON 0.000001f

using namespace glm;

class Object {

private:

	vec3 _ambient;
	vec3 _diffuse;
	vec3 _specular;
	vec3 _emission;
	GLfloat _shininess;

	mat4 _transform;
	mat4 _invTransform;	  	 // We compute it once, instead of each intersection test
	mat3 _invTransposeTrans; // For normals transforms - notice 3x3

protected:


public:


	Object();
	virtual ~Object();

	virtual bool intersectsRay(Ray &r, GLfloat &dist, vec3& point, vec3& normal, vec3& color) = 0;

	friend std::ostream& operator<< (std::ostream& out, const Object & obj);

	virtual const void print() const = 0;

	vec3& ambient () { return _ambient; }
	vec3& diffuse () { return _diffuse; }
	vec3& specular() { return _specular; }
	vec3& emission() { return _emission; }
	GLfloat& shininess() { return _shininess; }

	mat4& transform() { return _transform; }
	mat4& invTransform() { return _invTransform; }
	mat3& invTransposeTrans() { return _invTransposeTrans; }


protected:
	const GLfloat INFINITE = FLT_MAX;

};




#endif /* OBJECT_H_ */
