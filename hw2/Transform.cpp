// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	glm::mat3 ret;

	float c = glm::cos(glm::radians(degrees));
	float s = glm::sin(glm::radians(degrees));
	float t = 1 - c;

	ret[0][0] = t * axis.x * axis.x + c;
	ret[0][1] = t * axis.x * axis.y - s * axis.z;
	ret[0][2] = t * axis.x * axis.z + s * axis.y;

	ret[1][0] = t * axis.x * axis.y + s * axis.z;
	ret[1][1] = t * axis.y * axis.y + c;
	ret[1][2] = t * axis.y * axis.z - s * axis.x;

	ret[2][0] = t * axis.x * axis.z - s * axis.y;
	ret[2][1] = t * axis.y * axis.z + s * axis.x;
	ret[2][2] = t * axis.z * axis.z + c;

	return ret;
}

#include <iostream>
void Transform::left(float degrees, vec3& eye, vec3& up) 
{
	std::cout << "JERE" << std::endl;

	eye = Transform::rotate(-degrees, up) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
	// Rotate by the perpendicular axis to eye and up (obtain via cross-product)
	eye = Transform::rotate(-degrees, glm::normalize(glm::cross(eye, up))) * eye;
	// Update the new up vector
	up = glm::normalize(glm::cross(glm::cross(eye, up), eye));
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
	glm::vec3 w, u, v;
	glm::mat4x4 ret;

	static const glm::vec3 origin = vec3(0.0f, 0.0f, 0.0f);
	w = glm::normalize(eye);
	u = glm::normalize(glm::cross(up, w));
	v = glm::cross(w,u);

	ret[0][0] = u.x;
	ret[1][0] = u.y;
	ret[2][0] = u.z;
	ret[3][0] = -glm::dot(u, eye);

	ret[0][1] = v.x;
	ret[1][1] = v.y;
	ret[2][1] = v.z;
	ret[3][1] = -glm::dot(v, eye);

	ret[0][2] = w.x;
	ret[1][2] = w.y;
	ret[2][2] = w.z;
	ret[3][2] = -glm::dot(w, eye);

	ret[0][3] = 0;
	ret[1][3] = 0;
	ret[2][3] = 0;
	ret[3][3] = 1;


	return glm::lookAt(eye, center, up);
	//return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	mat4 ret = glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
	// YOUR CODE FOR HW2 HERE
	// New, to implement the perspective transform as well.
	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
	return glm::scale(mat4(1.0), glm::vec3(sx, sy, sz));

//	mat4 ret;
//	ret[0][0] = sx;
//	ret[1][1] = sy;
//	ret[2][2] = sz;
//	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	return glm::translate(mat4(1.0), glm::vec3(tx, ty, tz));

//	mat4 ret;
//	ret[0][3] = tx;
//	ret[1][3] = ty;
//	ret[2][3] = tz;
//	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
	vec3 x = glm::cross(up,zvec);
	vec3 y = glm::cross(zvec,x);
	vec3 ret = glm::normalize(y);
	return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
