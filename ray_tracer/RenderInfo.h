/*
 * RenderInfo.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef RENDERINFO_H_
#define RENDERINFO_H_

#include <GL/glew.h>
#include "Camera.h"
#include "Scene.h"


struct RenderInfo {

	GLuint width;
	GLuint height;

	GLuint maxDepth;

	std::string outputFile;

	Camera camera;

	Scene scene;


};




#endif /* RENDERINFO_H_ */
