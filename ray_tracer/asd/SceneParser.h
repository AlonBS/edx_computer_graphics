// Readfile definitions


#ifndef __SCENE_PARSER__
#define __SCENE_PARSER__

#include <GL/glew.h>

#include "Camera.h"
#include "RenderInfo.h"


//void matransform (stack<mat4> &transfstack, GLfloat * values) ;
//void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
//bool readvals (stringstream &s, const int numvals, GLfloat * values) ;

void readFile(const char* fileName, RenderInfo &renderInfo);
bool readValues(std::stringstream &s, const int numValues, GLfloat *values);






#endif //__SCENE_PARSER__
