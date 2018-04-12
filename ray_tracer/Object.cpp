/*
 * Object.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#include "Object.h"

Object::Object() {

	_properties._ambient = vec3(0.0f, 0.0f, 0.0f);
	_properties._emission = vec3(0.0f, 0.0f, 0.0f);
	_properties._diffuse = vec3(0.0f, 0.0f, 0.0f);
	_properties._specular = vec3(0.0f, 0.0f, 0.0f);
	_properties._shininess = 0.0f;

	_transform = mat4(1.0f);
	_invTransform = mat4(1.0f);
	_invTransposeTrans = mat3(1.0f);

	_texture = nullptr;
	_textured = false;

}

Object::~Object() {
	// TODO Auto-generated destructor stub
}

std::ostream& operator<< (std::ostream& out, const Object & obj)
{
	obj.print();
	return out;
}


void Object::setTexture(Image *texture)
{
	if (texture == nullptr)
		return;

	this->_texture = texture;
	this->_textured = true;

}

vec3 Object::getTextureColor(vec2& uv)
{
	int w = this->_texture->getWidth();
	int h = this->_texture->getHeight();

	/* TODO - consider interpolation */
	return this->_texture->getPixel((int)(uv.x * w), (int) (uv.y * h));
}



const void Object::print() const
{
	printVec3("Ambient", _properties._ambient);
	printVec3("_emission", _properties._emission);
	printVec3("_diffuse", _properties._diffuse);
	printVec3("_specular", _properties._specular);
	cout << "Shininess: " << _properties._shininess << endl;
}

