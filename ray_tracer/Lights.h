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

typedef struct Attenuation {

	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

}Attenuation;

class Light {

private:
	vec3 color;

public:

	Light(vec3& color): color(color) {}
	virtual ~Light() {}

};

class PointLight : Light {

private:

	vec3 position;

public:

	PointLight(vec3& color, vec3& pos) : Light(color), position(pos) {}
	virtual ~PointLight();
};

class DirectionalLight : Light {

private:
	vec3 direction;

public:

	DirectionalLight(vec3& color, vec3& dir) : Light(color), direction(dir) {}
	virtual ~DirectionalLight();
};





#endif /* LIGHTS_H_ */
