/*
 * Sphere.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#include "Sphere.h"

using namespace glm;



bool Sphere::intersectsRay(Ray &r, GLfloat &dist, vec3& normal)
{
	// To find intersection between Ray and Sphere represented the following way:
	// 	Sphere: (P - C )^2 - r^2 = 0
	// 	Ray:    (o + d*x)
	// we must solve a quadratic equations the form of Ax^2 + Bx + C = 0, where:
	// 	A: d^2
	//  B: 2 * d * (o - c)
	// 	C: (o -c)^2 - r^2

	GLfloat A, B, C;
	GLfloat discriminant, disc_root;
	GLfloat x1, x2, x;
	vec3    intersection_point;

	A = glm::dot(r.direction, r.direction);
	B = 2 * glm::dot(r.direction, (r.origin - center));
	C = glm::dot((r.origin - center), (r.origin - center)) - (radius * radius);

	discriminant = B*B - 4*A*C;
	if (discriminant < 0) {
		// No intersection
		return false;
	}

	disc_root = glm::sqrt(discriminant);
	x1 = (-B + disc_root) / (2*A);
	x2 = (-B - disc_root) / (2*A);

	if (x1 - x2 < EPSILON) {
		// same solution
		x = x1;
	}
	else if (x1 > EPSILON && x2 > EPSILON) {
		// Both positive
		x = glm::min(x1, x2);
	}
	else {
		// One positive, one negative
		// or 2 negatives
		x = glm::max(x1, x2);
	}

	dist = x;

	if (dist < EPSILON) {
		// If dist is a negative values (accounting for floating point errors)
		// then both solutions were negative. Meaning we have to go back from the origin of
		// the ray (against its direction) to the intersection point - which means of course that
		// there's no intersection.
		return false;
	}
	intersection_point = r.origin + x * r.direction;
	normal = glm::normalize(intersection_point - center);
	return true;
}

const void Sphere::print() const
{
	std::cout << "Center: (" << center.x << "," << center.y << "," << center.z << ") | Radius: " << radius << std::endl;

}

