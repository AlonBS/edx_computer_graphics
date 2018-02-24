/*
 * Triangle.cpp
 *
 *  Created on: Jan 26, 2018
 *      Author: alonbs
 */

#include "Triangle.h"

Triangle::Triangle(vec3& va, vec3& vb, vec3& vc)
: Object(), A(va), B(vb), C(vc)
{
	N = normalize(cross(C-B,A-B)); // Compute the face normal
	AN = vec3(0.0f, 0.0f, 0.0f);
	BN = vec3(0.0f, 0.0f, 0.0f);
	CN = vec3(0.0f, 0.0f, 0.0f);

}

Triangle::Triangle(vec3& va, vec3& vb, vec3& vc, vec3& vaNorm, vec3& vbNorm, vec3& vcNorm)
: Object(), A(va), B(vb), C(vc), AN(vaNorm), BN(vbNorm), CN(vcNorm)
{
	N = normalize(cross(C-B,A-B)); // Compute the face normal

}

Triangle::~Triangle()
{
}


bool Triangle::intersectsRay(Ray &r, GLfloat &dist, vec3& point, vec3& normal)
{
	GLfloat dist1, dist2;
	vec3    point1, point2;
	vec3	norm1, norm2;
	bool    res1, res2;

	// This is used for implementation stages - we use the other intersection as back up -
	// will be removed later on
//	res1 = __iRay(r, dist1, point1, norm1);
	res2 = __iRay2(r, dist2, point2, norm2);
//
//	assert(res1 == res2);
//
//	if (res1 != res2) {
//		std::cout << "Result of intersections differ: " << res1 << " " << res2 << std::endl;
//		exit(-1);
//	}
//
//	if (glm::abs(dist1 - dist2) > EPSILON)  {
//		std::cout << "DIst 1 and 2 differ: " << dist1 << " " << dist2 << std::endl;
//		exit(-1);
//	}

//	if (glm::abs(glm::length(point1) - glm::length(point2) > EPSILON)  {
//		std::cout << "Points differ: " << std::endl;
//		std::cout << "POINT1: (" << point1.x << "," << point1.y << "," << point1.z << ")" << std::endl;
//		std::cout << "POINT2: (" << point2.x << "," << point2.y << "," << point2.z << ")" << std::endl;
//		exit(-1);
//	}

	//
//
//	if (glm::abs(glm::length(norm1) - glm::length(norm2)) > EPSILON)  {
//		std::cout << "Normal differ: " << std::endl;
//		std::cout << "NORMAL1: (" << norm1.x << "," << norm1.y << "," << norm1.z << ")" << std::endl;
//		std::cout << "NORMAL2: (" << norm2.x << "," << norm2.y << "," << norm2.z << ")" << std::endl;
//		exit(-1);
//	}


	dist = dist2;
	point = point2;
	normal = norm2;
	return res2;
}


bool Triangle::__iRay(Ray &r, GLfloat &dist, vec3& point, vec3& normal)
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
	GLfloat	dot_ACAC, dot_ACAB, dot_ACAP, dot_ABAB, dot_ABAP;
	GLfloat invDenom, beta, gamma;



	// First - find intersection point
	d_dot_n = dot(r.direction, N);
	if (d_dot_n - EPSILON == 0 ) {
		// No intersection, or very close to no intersection
		return false;
	}

	a_dot_n = dot(A, N);
	o_dot_n = dot(r.origin, N);
	t = (a_dot_n - o_dot_n) / d_dot_n;

	if (t < EPSILON) {
		// if t < 0, then the triangle is behind the ray, thus no intersection
		return false;
	}

	// Now check if intersection point given by: o + td is inside the triangle.
	// We do this using barycentric coordinates
	// That is that:
	// 		(P-A) = b(B-A)+y(C-A). WHere P is the intersection point,
	//		 A, B, C are this triangles vertices, and a,b,y are alpha beta and gamma from barycentric coordinates.
	P = r.origin + t*r.direction;

	// Compute vectors
	AC = C - A;
	AB = B - A;
	AP = P - A;

	// Compute dot products


	dot_ACAC = dot(AC, AC);
	dot_ACAB = dot(AC, AB);
	dot_ACAP = dot(AC, AP);
	dot_ABAB = dot(AB, AB);
	dot_ABAP = dot(AB, AP);

	// Compute barycentric coordinates
	invDenom = 1 / (dot_ACAC * dot_ABAB - dot_ACAB * dot_ACAB);
	beta = (dot_ABAB * dot_ACAP - dot_ACAB * dot_ABAP) * invDenom;
	gamma = (dot_ACAC * dot_ABAP - dot_ACAB * dot_ACAP) * invDenom;

	// Check if point is in triangle
	if ( (beta >= 0) && (gamma >= 0) && (beta + gamma < 1) ){
		dist = t;
		point = P;
		normal = N;
		return true;
	}

	// No intersection - make sure values are irrelevant
	return false;
}


bool Triangle::__iRay2(Ray &r, GLfloat &dist, vec3& point, vec3& normal)
{
	// Another close computation - to check validity of the other

	GLfloat a_dot_n;
	GLfloat o_dot_n;
	GLfloat d_dot_n;
	GLfloat t = 0;
	vec3    P; // Intersection point

	vec3 APn, BPn, CPn;
	GLfloat APw, BPw, CPw;
	GLfloat alpha, beta, gamma;

	normal = vec3(0.0f, 0.0f, 0.0f);

	// First - find intersection point
	d_dot_n = dot(r.direction, N);
	if (d_dot_n - EPSILON == 0 ) {
		// No intersection, or very close to no intersection
		return false;
	}

	a_dot_n = dot(A, N);
	o_dot_n = dot(r.origin, N);
	t = (a_dot_n - o_dot_n) / d_dot_n;

	if (t < EPSILON) {
		// if t < 0, then the triangle is behind the ray, thus no intersection
		return false;
	}


	// Now check if intersection point given by: o + td is inside the triangle.
	// We do this using barycentric coordinates
	// That is that:
	// 		(P-A) = b(B-A)+y(C-A). WHere P is the intersection point,
	//		 A, B, C are this triangles vertices, and a,b,y are alpha beta and gamma from barycentric coordinates.
	P = r.origin + t*r.direction;

	APn = cross(N, C-B) / (dot(cross(N, C-B), A-C));
	APw = dot(-APn, C);
	BPn = cross(N, A-C) / (dot(cross(N, A-C), B-A));
	BPw = dot(-BPn, A);
	CPn = cross(N, B-A) / (dot(cross(N, B-A), C-B));
	CPw = dot(-CPn, B);

	alpha = dot(APn, P) + APw;
	beta  = dot(BPn, P) + BPw;
	gamma = dot(CPn, P) + CPw;

	if (alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1 && gamma >= 0 && gamma <= 1) {
		dist = t;
		point = P;
		normal = this->N;
		return true;
	}

	return false;

}


/*
bool Triangle::__iRay3(Ray &r, GLfloat &dist, glm::vec3& normal)
{
	// Another close computation - to check validity of the other

	GLfloat a_dot_n;
	GLfloat o_dot_n;
	GLfloat d_dot_n;
	GLfloat t = 0;
	vec3    P; // Intersection point
	vec3    AB, AC;
	vec3    PA, PB, PC;
	GLfloat ABC2, alpha, beta, gamma;

	normal = vec3(0.0f, 0.0f, 0.0f);

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
		// No intersection - make sure values are irrelevant
		dist = 0;
		normal = vec3(0.0f, 0.0f, 0.0f);
		return false;
	}

	dist = t;
	normal = N;
	return true;
}
*/


vec3 Triangle::getTextureColor(vec3& point)
{
	return VECTOR_WHITE;
}

const void Triangle::print() const
{

	cout << "TRIANGLE" << endl;
	Object::print();
}


