/*
 * General.h
 *
 *  Created on: Feb 10, 2018
 *      Author: alonbs
 */

#ifndef GENERAL_H_
#define GENERAL_H_

#include <cstdint>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <iostream>

const GLfloat EPSILON  = 0.001f;
const GLfloat INFINITE = FLT_MAX;
const GLfloat PI = 3.1415926f;

using namespace glm;
using namespace std;

inline void
printVec3(const string& name, const vec3& vec)
{
	cout << name << ": " << vec.x << "," << vec.y << "," << vec.z << endl;
}

inline void
printMat4(const string& name, const mat4& mat)
{
	cout << name << ":" << endl;
	for (int i = 0; i < 4 ; ++i) {
		for (int j = 0 ; j < 4 ; ++j) {

			cout << "\t" << mat[i][j];
		}
		cout << endl;
	}
}




#endif /* GENERAL_H_ */
