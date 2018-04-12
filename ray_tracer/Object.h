/*
 * Object.h
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#ifndef OBJECT_H_
#define OBJECT_H_

#include "Ray.h"
#include <GL/glew.h>
#include "General.h"
#include "Image.h"

using namespace glm;

typedef struct ObjectProperties {

	vec3 _ambient;
	vec3 _emission;
	vec3 _diffuse;
	vec3 _specular;
	GLfloat _shininess;

}ObjectProperties;

class Object {

private:



//	vec3 _ambient;
//	vec3 _diffuse;
//	vec3 _specular;
//	vec3 _emission;
//	GLfloat _shininess;

	mat4 _transform;
	mat4 _invTransform;	  	 // We compute it once, instead of each intersection test
	mat3 _invTransposeTrans; // For normals transforms - notice 3x3



protected:

	ObjectProperties _properties;

	bool _textured;
	Image *_texture;

	vec3 getTextureColor(vec2& uv);

public:


	Object();
	virtual ~Object();

	void setTexture(Image *texture);

	virtual bool intersectsRay(Ray &r, GLfloat &dist, vec3* point, vec3* normal, vec3* texColor, ObjectProperties* properties) = 0;



	friend std::ostream& operator<< (std::ostream& out, const Object & obj);

	virtual const void print() const;

	ObjectProperties& properties() { return _properties; };
	vec3& ambient () { return _properties._ambient; }
	vec3& diffuse () { return _properties._diffuse; }
	vec3& specular() { return _properties._specular; }
	vec3& emission() { return _properties._emission; }
	GLfloat& shininess() { return _properties._shininess; }

	mat4& transform() { return _transform; }
	mat4& invTransform() { return _invTransform; }
	mat3& invTransposeTrans() { return _invTransposeTrans; }

};




#endif /* OBJECT_H_ */
