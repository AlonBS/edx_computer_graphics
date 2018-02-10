/*
 * Lights.h
 *
 *  Created on: Feb 10, 2018
 *      Author: alonbs
 */


#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Lights.h"

using namespace glm;


Light::Light(vec3& color)
:color(color)
{
}

Light::~Light()
{
}


PointLight::PointLight(vec3& color, vec3& pos)
: Light(color), position(pos)
{
}


DirectionalLight::DirectionalLight(vec3& color, vec3& dir)
: Light(color), direction(dir)
{
}
