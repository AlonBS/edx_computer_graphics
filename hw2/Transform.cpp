// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
	glm::mat3 ret;
	vec3 an = glm::normalize(axis); // normalize the axis first.
	float c = glm::cos(glm::radians(degrees));
	float s = glm::sin(glm::radians(degrees));
	float t = 1 - c;

	ret[0][0] = t * an.x * an.x + c;
	ret[0][1] = t * an.x * an.y - s * an.z;
	ret[0][2] = t * an.x * an.z + s * an.y;

	ret[1][0] = t * an.x * an.y + s * an.z;
	ret[1][1] = t * an.y * an.y + c;
	ret[1][2] = t * an.y * an.z - s * an.x;

	ret[2][0] = t * an.x * an.z - s * an.y;
	ret[2][1] = t * an.y * an.z + s * an.x;
	ret[2][2] = t * an.z * an.z + c;

	// easier to transpose here than on each row separately
	return ret;
}


void Transform::left(float degrees, vec3& eye, vec3& up) 
{
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

	return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	mat4 ret(0);

	float tanHalfAng = tan(glm::radians(fovy) / 2);
	ret[0][0] = 1 / (aspect * tanHalfAng);
	ret[1][1] = 1 / tanHalfAng;
	ret[2][2] = - ((zFar + zNear) / (zFar - zNear));
	ret[2][3] = -1;
	ret[3][2] = - ((2 * zFar * zNear) / (zFar - zNear));

	return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
	mat4 ret;
	// column-major order in open-GL, but this is the diagonal anyway.
	ret[0][0] = sx;
	ret[1][1] = sy;
	ret[2][2] = sz;
	return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
	mat4 ret;
	// column-major order in open-GL, so we 'flip' lines.
	ret[3][0] = tx;
	ret[3][1] = ty;
	ret[3][2] = tz;
	return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
	vec3 up_1 = glm::cross(up, zvec);
	vec3 up_2 = glm::cross(zvec, up_1);
	return glm::normalize(up_2);
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
