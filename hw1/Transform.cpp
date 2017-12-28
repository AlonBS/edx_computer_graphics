// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

mat3 Transform::rotate(const float degrees, const vec3& axis) {

	glm::mat3 res;

	float c = glm::cos(glm::radians(degrees));
	float s = glm::sin(glm::radians(degrees));
	float t = 1 - c;

	res[0][0] = t * axis.x * axis.x + c;
	res[0][1] = t * axis.x * axis.y - s * axis.z;
	res[0][2] = t * axis.x * axis.z + s * axis.y;

	res[1][0] = t * axis.x * axis.y + s * axis.z;
	res[1][1] = t * axis.y * axis.y + c;
	res[1][2] = t * axis.y * axis.z - s * axis.x;

	res[2][0] = t * axis.x * axis.z - s * axis.y;
	res[2][1] = t * axis.y * axis.z + s * axis.x;
	res[2][2] = t * axis.z * axis.z + c;

	return res;
}

void Transform::left(float degrees, vec3& eye, vec3& up) {

	eye = Transform::rotate(degrees, -up) * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up) {

	// Rotate by the perpendicular axis to eye and up (obtain via cross-product)
	eye = Transform::rotate(degrees, glm::normalize(glm::cross(eye, -up))) * eye;
	// Update the new up vector
	up = glm::normalize(glm::cross(glm::cross(eye, up), eye));
}

mat4 Transform::lookAt(vec3 eye, vec3 up) {

	glm::vec3 w, u, v;
	glm::mat4x4 res;

	static const glm::vec3 origin = vec3(0.0f, 0.0f, 0.0f);
	w = glm::normalize(eye);
	u = glm::normalize(glm::cross(up, w));
	v = glm::cross(w,u);

	res[0][0] = u.x;
	res[1][0] = u.y;
	res[2][0] = u.z;
	res[3][0] = -glm::dot(u, eye);

	res[0][1] = v.x;
	res[1][1] = v.y;
	res[2][1] = v.z;
	res[3][1] = -glm::dot(v, eye);

	res[0][2] = w.x;
	res[1][2] = w.y;
	res[2][2] = w.z;
	res[3][2] = -glm::dot(w, eye);

	res[0][3] = 0;
	res[1][3] = 0;
	res[2][3] = 0;
	res[3][3] = 1;

	return res;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
