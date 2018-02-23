/*
 * Object.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#include "Object.h"

Object::Object() {

	_ambient = vec3(0.0f, 0.0f, 0.0f);
	_diffuse = vec3(0.0f, 0.0f, 0.0f);
	_specular = vec3(0.0f, 0.0f, 0.0f);
	_emission = vec3(0.0f, 0.0f, 0.0f);
	_shininess = 0.0f;

	_transform = mat4(1.0f);
	_invTransform = mat4(1.0f);
	_invTransposeTrans = mat3(1.0f);

	texture = nullptr;

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

std::ostream& operator<< (std::ostream& out, const Object & obj)
{
	obj.print();
	return out;
}

const void Object::print() const
{
	printVec3("Ambient", _ambient);
	printVec3("_diffuse", _diffuse);
	printVec3("_specular", _specular);
	printVec3("_emission", _emission);
	cout << "Shininess: " << _shininess << endl;
}

