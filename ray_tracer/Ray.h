/*
 * Ray.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef RAY_H_
#define RAY_H_

#include <glm/glm.hpp>

class Ray {

private:

	glm::vec3 origin;
	glm::vec3 direction;

public:
	Ray(glm::vec3 origin, glm::vec3 direction);
	virtual ~Ray();
};

#endif /* RAY_H_ */
