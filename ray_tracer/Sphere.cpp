/*
 * Sphere.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#include "Sphere.h"

using namespace glm;



bool Sphere::intersectsRay(Ray &r, GLfloat &dist, vec3& point, vec3& normal)
{
	// To find intersection between Ray and Sphere represented the following way:
	// 	Sphere: (P - C )^2 - r^2 = 0
	// 	Ray:    (o + d*x)
	// we must solve a quadratic equations the form of Ax^2 + Bx + C = 0, where:
	// 	A: d^2
	//  B: 2 * d * (o - c)
	// 	C: (o -c)^2 - r^2

	Ray tr = this->invTransform() * r; // Transformed ray
	GLfloat A, B, C;
	GLfloat discriminant, disc_root;
	GLfloat x1, x2, x;
	vec3    intersection_point;


	A = glm::dot(tr.direction, tr.direction);
	B = 2 * glm::dot(tr.direction, (tr.origin - center));
	C = glm::dot((tr.origin - center), (tr.origin - center)) - (radius * radius);

	discriminant = B*B - 4*A*C;
	if (discriminant < 0) {
		// No intersection
		return false;
	}

	disc_root = glm::sqrt(discriminant);
	x1 = (-B + disc_root) / (2*A);
	x2 = (-B - disc_root) / (2*A);

	if (abs(x1 - x2) < EPSILON) {
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

	if (x < 0) {
		// If dist is a negative values (accounting for floating point errors)
		// then both solutions were negative. Meaning we have to go back from the origin of
		// the ray (against its direction) to the intersection point - which means of course that
		// there's no intersection.
		return false;
	}


	intersection_point = tr.origin + x * tr.direction;
	vec3 n = vec3(intersection_point - center);

	// M * p - to transform point back
	intersection_point = vec3(this->transform() * vec4(intersection_point, 1.0f));
	// The distance is the length of the original intersection point with the origin of the non transformed ray.
	dist = length(intersection_point - r.origin);
	point = intersection_point;
	normal = normalize(vec3(mat3(this->invTransposeTrans()) * n));
	return true;
}

const void Sphere::print() const
{
	std::cout << "Center: (" << center.x << "," << center.y << "," << center.z << ") | Radius: " << radius << std::endl;
	Object::print();
}


/*
vec3 center = vec3(0.0f, 0.0f, 0.0f);
	GLfloat radius = 1;
	Sphere ss(center, radius);
	GLfloat dist;
	vec3 normal;

	vec3 tt = glm::normalize(glm::vec3(5.0, 3, 2));
	cout << "DOTTT: " << glm::dot(tt,tt) << endl;


	vec3 o1 = vec3(0, 0, -0.5); vec3 d1 = vec3(0, 0, 1);
	vec3 o2 = vec3(0, -2, 0); vec3 d2 = vec3(0, 1, 0);
	vec3 o3 = vec3(-2, 0, 0); vec3 d3 = vec3(1, 0, 0);

	vec3 o4 = vec3(0, 0, -0.5); vec3 d4 = vec3(0, 0, -1);
	vec3 o5 = vec3(0, -2, 0); vec3 d5 = vec3(0, -1, 0);
	vec3 o6 = vec3(-2, 0, 0); vec3 d6 = vec3(-1, 0, 0);


	Ray r1(o1, d1), r2(o2, d2), r3(o3, d3);
	Ray r4(o4, d4), r5(o5, d5), r6(o6, d6);


	std::vector<Ray> vecs;
	vecs.push_back(r1); vecs.push_back(r2); vecs.push_back(r3);
	vecs.push_back(r4); vecs.push_back(r5); vecs.push_back(r6);

	for (Ray r : vecs) {

		if (ss.intersectsRay(r, dist, normal) ) {

			cout << "Int. Dist: " << dist << " . Normal: " << normal.x << ", " << normal.y << " ," << normal.z << "." << std::endl;
		}
		else {
			std::cout << "No intersection!" << std::endl;
		}

	}
 */
