/*
 * Triangle.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: alonbs
 */

#include "Triangle.h"

Triangle::Triangle(vec3& va, vec3& vb, vec3& vc)
: A(va), B(vb), C(vc)
{
	N = normalize(cross(C-A,B-A)); // Compute the face normal
}

Triangle::~Triangle()
{
}


bool Triangle::intersectsRay(Ray &r, GLfloat &dist, glm::vec3& normal)
{

	// We do that in two steps:
	// 	- First, we intersect the ray with the plane this triangle lays in
	//  - If there's an intersection - the we check if the intersection point is within our triangle


	// First - Plane-Ray intersection
	// Plane: (P-A)*N = 0. --> (P*N)-(A*N) = 0 (where P is a point on plane, and A is one of the triangles point)
	// Ray  : P = O + d*t
	// We combine the equations and solve for t: (O + dt)*N = An.
	// We re arrange and get:        A*N-O*N
	//							t =	---------
	//								  D*N

	GLfloat a_dot_n;
	GLfloat o_dot_n;
	GLfloat d_dot_n;
	GLfloat t;
	vec3    P; // Intersection point
	vec3    AP, AB, AC;
	GLfloat alpha, beta, gamma;

	// First - find intersection point
	d_dot_n = dot(r.direction, N);
	if (d_dot_n - EPSILON == 0 ) {
		// No intersection, or very close to no intersection
		return false;
	}

	a_dot_n = dot(A, N);
	o_dot_n = dot(r.origin, N);
	t = (a_dot_n - o_dot_n) / d_dot_n;

	// Now check if intersection point given by: o + td is inside the triangle.
	// We do this using barycentric coordinates
	// That is that:
	// 		(P-A) = b(B-A)+y(C-A). WHere P is the intersection point,
	//		 A, B, C are this triangles vertices, and a,b,y are alpha beta and gamma from barycentric coordinates.
	P = r.origin + t*r.direction;



	// After some algebra, we get the following equation: gamma  = (PAYBAX-BAYPAX) / (CAYBAX-CAXBAY)
	AP = P-A; AB = B-A; AC = C-A;

	std::cout << AP.y << std::endl;
	std::cout << AB.x << std::endl;
	std::cout << AB.y << std::endl;
	std::cout << AP.x << std::endl;

	std::cout << (AP.y*AB.x)-(AB.y*AP.x) << std::endl;
	std::cout << (AC.y*AB.x)-(AC.x*AB.y) << std::endl;

	gamma = ( (AP.y*AB.x)-(AB.y*AP.x) )/( (AC.y*AB.x)-(AC.x*AB.y) );
	beta = (AP.z-(gamma*AC.z))/AB.z;

	std::cout<< " BETA 1 " << beta << std::endl;
	std::cout<< " GAMMA 1 " << gamma << std::endl;

	if (gamma >= 0 && beta >= 0 && gamma+beta <= 1) {

		normal = N;
		dist = t;
		return true;
	}

	return false;
}


bool Triangle::intersectsRay2(Ray &r, GLfloat &dist, glm::vec3& normal)
{

	// Anoter close computation - to check validity of the other

	GLfloat a_dot_n;
	GLfloat o_dot_n;
	GLfloat d_dot_n;
	GLfloat t;
	vec3    P; // Intersection point
	vec3    AB, AC;
	vec3    PA, PB, PC;
	GLfloat ABC2, alpha, beta, gamma;

	// First - find intersection point
	d_dot_n = dot(r.direction, N);
	if (d_dot_n - EPSILON == 0 ) {
		// No intersection, or very close to no intersection
		return false;
	}

	a_dot_n = dot(A, N);
	o_dot_n = dot(r.origin, N);
	t = (a_dot_n - o_dot_n) / d_dot_n;

	// Now check if intersection point given by: o + td is inside the triangle.
	// We do this using barycentric coordinates
	// That is that:
	// 		(P-A) = b(B-A)+y(C-A). WHere P is the intersection point,
	//		 A, B, C are this triangles vertices, and a,b,y are alpha beta and gamma from barycentric coordinates.
	P = r.origin + t*r.direction;

	AB = B-A; AC = C-A;
	PA = A-P; PB = B-P; PC = C-P;

	ABC2 = length(cross(AB, AC));
	alpha = length(cross(PB, PC)) / (ABC2);
	beta = length(cross(PC, PA)) / (ABC2);
	gamma = 1 - alpha - beta;

	if ( alpha < 0 || alpha > 1 || beta < 0 || beta > 1 || gamma < 0 || gamma > 1) {
		return false;
	}

	dist = t;
	normal = N;
	return true;
}


