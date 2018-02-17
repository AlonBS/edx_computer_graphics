/*
 * Lights.h
 *
 *  Created on: Feb 10, 2018
 *      Author: alonbs
 */

#ifndef LIGHTS_H_
#define LIGHTS_H_

#include <glm/glm.hpp>
#include <GL/glew.h>

using namespace glm;

typedef struct Attenuation {

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

}Attenuation_t;

class Light {

public:

	vec3 _color;

	Light(vec3& color);
	virtual ~Light() {};

};

class PointLight : public Light {

public:
	vec3 _position;

	PointLight(vec3& color, vec3& pos);
	virtual ~PointLight() {}
};

class DirectionalLight : public Light {

public:
	vec3 _direction;


	DirectionalLight(vec3& color, vec3& dir);
	virtual ~DirectionalLight() {}
};





#endif /* LIGHTS_H_ */
