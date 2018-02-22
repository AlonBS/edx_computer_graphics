/*****************************************************************************/
/* This is the program skeleton for homework 2 in CSE167 by Ravi Ramamoorthi */
/* Extends HW 1 to deal with shading, more transforms and multiple objects   */
/*****************************************************************************/

/*****************************************************************************/
// This file is readfile.cpp.  It includes helper functions for matrix 
// transformations for a stack (matransform) and to rightmultiply the 
// top of a stack.  These functions are given to aid in setting up the 
// transformations properly, and to use glm functions in the right way.  
// Their use is optional in your program.  


// The functions readvals and readfile do basic parsing.  You can of course 
// rewrite the parser as you wish, but we think this basic form might be 
// useful to you.  It is a very simple parser.

// Please fill in parts that say YOUR CODE FOR HW 2 HERE. 
// Read the other parts to get a context of what is going on. 

/*****************************************************************************/

// Basic includes to get this file to work.  
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <GL/glew.h>
#include <GL/glut.h>

#include "SceneParser.h"
#include "Camera.h"
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace std;




struct Commands {

	// General Scene
	const string size          = "size";
	const string maxdepth      = "maxdepth";
	const string output        = "output";

	// Camera
	const string camera        = "camera";

	// Geometry and objects
	const string sphere        = "sphere";
	const string maxverts      = "maxverts";
	const string maxvertnorms  = "maxvertnorms";
	const string vertex        = "vertex";
	const string vertexnormal  = "vertexnormal";
	const string tri           = "tri";
	const string trinormal     = "trinormal";

	// Transformations
	const string translate     = "translate";
	const string rotate        = "rotate";
	const string scale         = "scale";
	const string pushTransform = "pushTransform";
	const string popTransform  = "popTransform";

	// Lights
	const string directional   = "directional";
	const string point         = "point";
	const string attenuation   = "attenuation";

	// Materials
	const string ambient       = "ambient"; // As this is per object - and not per scene
	const string diffuse       = "diffuse";
	const string specular      = "specular";
	const string shininess     = "shininess";
	const string emission      = "emission";

}Commands;


/////////////////////////////////////INIT STATIC MEMBERS ///////////////////////////////////////////////////

set<string> SceneParser::general = {Commands.size, Commands.maxdepth, Commands.output};
string 	    SceneParser::camera = Commands.camera;
set<string> SceneParser::geometry = {Commands.sphere, Commands.maxverts, Commands.maxvertnorms,
									 Commands.vertex, Commands.vertexnormal, Commands.tri,
									 Commands.trinormal};
set<string> SceneParser::transformations = {Commands.translate, Commands.rotate, Commands.scale,
											Commands.pushTransform, Commands.popTransform};
set<string> SceneParser::lights = {Commands.directional, Commands.point, Commands.attenuation};
set<string> SceneParser::materials = {Commands.ambient, Commands.diffuse, Commands.specular, Commands.shininess, Commands.emission};


vec3 SceneParser::ambient  = vec3(0.2f, 0.2f, 0.2f);
vec3 SceneParser::diffuse  = vec3(0.0f, 0.0f, 0.0f);
vec3 SceneParser::specular = vec3(0.0f, 0.0f, 0.0f);
vec3 SceneParser::emission = vec3(0.0f, 0.0f, 0.0f);
GLfloat SceneParser::shininess = 0.0f;
GLfloat SceneParser::values[MAX_POSSIBLE_VALUES] = {};
RenderInfo SceneParser::renderInfo = {};

Attenuation SceneParser::attenuation = { .constant = 1.0f, .linear = 0.0f, .quadratic = 0.0f};
GLuint 		SceneParser::maxDepth = 1;

stack<mat4> SceneParser::transformsStack;


///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool
SceneParser::inSet(set<string>& set, string& cmd)
{
	return set.find(cmd) != set.end();
}
bool
SceneParser::isGeneralCommand(std::string& cmd)
{
	return inSet(general, cmd);
}

bool
SceneParser::isCameraCommand(std::string& cmd)
{
	return cmd == SceneParser::camera;
}
bool
SceneParser::isGeometryCommand(std::string& cmd)
{
	return inSet(geometry, cmd);
}
bool
SceneParser::isTransformationsCommand(std::string& cmd)
{
	return inSet(transformations, cmd);
}
bool
SceneParser::isLightsCommand(std::string& cmd)
{
	return inSet(lights, cmd);
}
bool
SceneParser::isMaterialsCommand(std::string& cmd)
{
	return inSet(materials, cmd);
}

CommandType
SceneParser::identifyCommand(std::string & cmd)
{
	if (isGeneralCommand(cmd)) {
		return GENERAL;
	}
	else if (isCameraCommand(cmd)) {
		return CAMERA;
	}
	else if (isGeometryCommand(cmd)) {
		return GEOMETRY;
	}
	else if (isTransformationsCommand(cmd)) {
		return TRANSFORMATIONS;
	}
	else if (isLightsCommand(cmd)) {
		return LIGHTS;
	}
	else if (isMaterialsCommand(cmd)) {
		return MATERIALS;
	}
	return UNKNOWN_COMMAND;
}




//cout << Commands::camera;


//// You may not need to use the following two functions, but it is provided
//// here for convenience
//
//// The function below applies the appropriate transform to a 4-vector
//void matransform(stack<mat4> &transfstack, GLfloat* values)
//{
//	mat4 transform = transfstack.top();
//	vec4 valvec = vec4(values[0],values[1],values[2],values[3]);
//	vec4 newval = transform * valvec;
//	for (int i = 0; i < 4; i++) values[i] = newval[i];
//}
//
void
rightMultiply(const mat4 & M, stack<mat4> &transfstack)
{
	mat4 &T = transfstack.top();
	T = T * M;

}

bool
SceneParser::readValues(stringstream &s, const int numOfVals, GLfloat* values)
{
	for (int i = 0; i < numOfVals; ++i) {
		s >> values[i];
		if (s.fail()) {
			cout << "Failed reading value " << i << " will skip\n";
			return false;
		}
	}
	return true;
}



RenderInfo
SceneParser::readFile(const char* fileName)
{
	string str, cmd;
	ifstream in;

	in.open(fileName);
	if (!in.is_open()) {

		std::cout << "Unable to open file for read" << std::endl;
		std::cout << "Given file name was: " << fileName << std::endl;
	}

	// Default transform
	transformsStack.push(mat4(1.0));

	// Default attenuation
	renderInfo.scene.Attenuation() = attenuation;
	renderInfo.maxDepth = maxDepth;

	while (in) {

		getline(in, str);

		// Ignore comments and white spaces of any kind
		if ( !((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#') ))  {
			continue;
		}

		stringstream s(str);
		s >> cmd;

		CommandType command = SceneParser::identifyCommand(cmd);
		switch (command) {
		case GENERAL:
			handleGeneralCommand(s, cmd);
			break;

		case CAMERA:
			handleCameraCommand(s, cmd);
			break;

		case GEOMETRY:
			handleGeometryCommand(s, cmd);
			break;

		case TRANSFORMATIONS:
			handleTransformationsCommand(s, cmd);
			break;

		case LIGHTS:
			handleLightsCommand(s, cmd);
			break;

		case MATERIALS:
			handleMaterialsCommand(s, cmd);
			break;

		case UNKNOWN_COMMAND:
		default:

			cout << "Unknown command: " << cmd << ".\n Skipped. " << endl;
			break;
		}

	}

	in.close();

	return SceneParser::renderInfo;
}




void
SceneParser::handleGeneralCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.size) {

		readValues(s, 2, values);
		renderInfo.width = values[0];
		renderInfo.height = values[1];
	} else if (cmd == Commands.maxdepth) {

		readValues(s, 1, values);
		renderInfo.maxDepth = values[0];

	} else if (cmd == Commands.output) {
		s >> renderInfo.outputFile;
	}

}


void
SceneParser::handleCameraCommand(stringstream& s, string& cmd)
{
	readValues(s, 10, values);
	vec3 eyeInit = glm::vec3(values[0], values[1], values[2]);
	vec3 center  = glm::vec3(values[3], values[4], values[5]);
	vec3 upInit  = glm::vec3(values[6], values[7], values[8]);
	//upinit = Transform::upvector(upinit, eyeinit);
	GLfloat fovy = values[9];
	renderInfo.camera = new Camera(eyeInit, center, upInit, fovy, renderInfo.width, renderInfo.height);
	//transformsStack.top() = lookAt(eyeInit,center,upInit);
}


void
SceneParser::handleGeometryCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.sphere) {
		readValues(s, 4, values);
		vec3 center = glm::vec3(values[0], values[1], values[2]);
		GLfloat radius = values[3];
		Object *sphere = new Sphere(center, radius);
		sphere->ambient() = ambient;
		sphere->specular() = specular;
		sphere->diffuse() = diffuse;
		sphere->emission() = emission;
		sphere->shininess() = shininess;
		sphere->transform() = transformsStack.top();
		sphere->invTransform() = inverse(sphere->transform());
		sphere->invTransposeTrans() = mat3(transpose(sphere->invTransform()));
		renderInfo.scene.addObject(sphere);

		sphere->print();
	}

	else if (cmd == Commands.maxverts) {
		// New object is coming
		renderInfo.vertcies.clear();
	}

	else if (cmd == Commands.maxvertnorms) {
		// New Object is coming
		renderInfo.vertciesNormals.clear();
	}

	else if (cmd == Commands.vertex) {
		readValues(s, 3, values);
		renderInfo.vertcies.push_back(vec3(values[0], values[1], values[2]));
	}

	else if (cmd == Commands.vertexnormal) {
		readValues(s, 6, values);
		renderInfo.vertciesNormals.push_back(vec3(values[0], values[1], values[2]));
		renderInfo.vertciesNormals.push_back(vec3(values[3], values[4], values[5]));
	}

	else if (cmd == Commands.tri) {
		readValues(s, 3, values);
		vec3 A = vec3 (transformsStack.top() * vec4(renderInfo.vertcies[values[0]], 1.0f));
		vec3 B = vec3 (transformsStack.top() * vec4(renderInfo.vertcies[values[1]], 1.0f));
		vec3 C = vec3 (transformsStack.top() * vec4(renderInfo.vertcies[values[2]], 1.0f));
		Object *triangle = new Triangle(A, B, C);

//		Object *triangle = new Triangle(renderInfo.vertcies[values[0]],
//												renderInfo.vertcies[values[1]],
//												renderInfo.vertcies[values[2]]);
		triangle->ambient() = ambient;
		triangle->specular() = specular;
		triangle->diffuse() = diffuse;
		triangle->emission() = emission;
		triangle->shininess() = shininess;

//		triangle->transform() = transformsStack.top();
//		triangle->invTransform() = inverse(triangle->transformMat);
//		triangle->invTransposeTrans() = transpose(triangle->invTransformMat);
		renderInfo.scene.addObject(triangle);
	}

	else if (cmd == Commands.trinormal) {
		readValues(s, 3, values);
		// TODO - need to transform normals too ?
		Object *triangle = new Triangle(renderInfo.vertcies[values[0] * 2],
				renderInfo.vertcies[values[1] * 2],
				renderInfo.vertcies[values[2] * 2],
				renderInfo.vertcies[values[0] * 2 - 1],
				renderInfo.vertcies[values[1] * 2 - 1],
				renderInfo.vertcies[values[2] * 2 - 1]);
		renderInfo.scene.addObject(triangle);
	}
}


void
SceneParser::handleTransformationsCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.translate) {
		readValues(s,3,values);
		transformsStack.top() = glm::translate(transformsStack.top(), vec3(values[0], values[1], values[2]));
	}

	else if (cmd == Commands.scale) {
		readValues(s,3,values);
		transformsStack.top() = glm::scale(transformsStack.top(), vec3(values[0], values[1], values[2]));
	}

	else if (cmd == Commands.rotate) {
		readValues(s,4,values);
		transformsStack.top() = glm::rotate(transformsStack.top(), radians(values[3]), vec3(values[0], values[1], values[2]));
	}

	else if (cmd == Commands.pushTransform) {
		transformsStack.push(transformsStack.top());
	}

	else if (cmd == Commands.popTransform) {
		if (transformsStack.size() <= 1) {
			cerr << "Stack has no elements.  Cannot Pop\n";
		} else {
			transformsStack.pop();
		}
	}
}


void
SceneParser::handleLightsCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.directional) {
		readValues(s, 6, values);
		vec3 dir = vec3(transformsStack.top() * vec4(values[0], values[1], values[2], 0.0f));
		vec3 color = vec3(values[3], values[4], values[5]);
		DirectionalLight *dirLight = new DirectionalLight(color, dir);
		renderInfo.scene.addDirectionalLight(dirLight);
	}

	else if (cmd == Commands.point) {
		readValues(s, 6, values);
		//TODO CHECK
		vec3 pos = vec3(transformsStack.top() * vec4(values[0], values[1], values[2], 1.0f));
		vec3 color = vec3(values[3], values[4], values[5]);
		PointLight *pointLight = new PointLight(color, pos);
		renderInfo.scene.addPointLight(pointLight);
	}

	else if (cmd == Commands.attenuation) {
		readValues(s, 3, values);
		Attenuation atten = {
				.constant = values[0],
				.linear = values[1],
				.quadratic = values[2]
		};

		renderInfo.scene.Attenuation() = atten;
	}

}


void
SceneParser::handleMaterialsCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.ambient) {
		readValues(s, 3, values);
		ambient = vec3(values[0], values[1], values[2]);
	}

	else if (cmd == Commands.diffuse) {
		readValues(s, 3, values);
		diffuse = vec3(values[0], values[1], values[2]);

	}

	else if (cmd == Commands.specular) {
		readValues(s, 3, values);
		specular = vec3(values[0], values[1], values[2]);
	}

	else if (cmd == Commands.emission) {
		readValues(s, 3, values);
		emission = vec3(values[0], values[1], values[2]);
	}

	else if (cmd == Commands.shininess) {
		readValues(s, 1, values);
		shininess = values[0];
	}
}


