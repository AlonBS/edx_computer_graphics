/*
 * Ray.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#include "Ray.h"

Ray::Ray(glm::vec3& origin, glm::vec3& direction)
:origin(origin), direction(glm::normalize(direction))
{
	// TODO - consider changing origin a bit - due to floating point errors

}

Ray::~Ray() {
	// TODO Auto-generated destructor stub
}

