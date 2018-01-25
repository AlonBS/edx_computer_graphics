/*
 * Object.cpp
 *
 *  Created on: Jan 25, 2018
 *      Author: alonbs
 */

#include "Object.h"

//Object::Object() {
//	// TODO Auto-generated constructor stub
//
//}
//
//Object::~Object() {
//	// TODO Auto-generated destructor stub
//}

std::ostream& operator<< (std::ostream& out, const Object & obj)
{
	obj.print();
	return out;
}

