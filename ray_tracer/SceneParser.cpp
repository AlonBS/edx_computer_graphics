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
#include <deque>
#include <stack>
#include <set>
#include <GL/glew.h>
#include <GL/glut.h>

#include "SceneParser.h"
#include "Camera.h"
#include "Object.h"
#include "Sphere.h"
#include "Triangle.h"

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


vec3 SceneParser::ambient = vec3(0.2f, 0.2f, 0.2f);
GLfloat SceneParser::values[MAX_POSSIBLE_VALUES] = {};
RenderInfo SceneParser::renderInfo = {};


///////////////////////////////////////////////////////////////////////////////////////////////////////////

bool
SceneParser::inSet(set<string>& set, string& cmd)
{
	return set.find(cmd) != set.end();
}
bool
SceneParser::isGeneralCommand(std::string& cmd)
{
	return SceneParser::inSet(SceneParser::general, cmd);
}

bool
SceneParser::isCameraCommand(std::string& cmd)
{
	return cmd == SceneParser::camera;
}
bool
SceneParser::isGeometryCommand(std::string& cmd)
{
	return SceneParser::inSet(SceneParser::geometry, cmd);
}
bool
SceneParser::isTransformationsCommand(std::string& cmd)
{
	return SceneParser::inSet(SceneParser::transformations, cmd);
}
bool
SceneParser::isLightsCommand(std::string& cmd)
{
	return SceneParser::inSet(SceneParser::lights, cmd);
}
bool
SceneParser::isMaterialsCommand(std::string& cmd)
{
	return SceneParser::inSet(SceneParser::materials, cmd);
}

CommandType
SceneParser::identifyCommand(std::string & cmd)
{
	if (SceneParser::isGeneralCommand(cmd)) {
		return GENERAL;
	}
	else if (SceneParser::isCameraCommand(cmd)) {
		return CAMERA;
	}
	else if (SceneParser::isGeometryCommand(cmd)) {
		return GEOMETRY;
	}
	else if (SceneParser::isTransformationsCommand(cmd)) {
		return TRANSFORMATIONS;
	}
	else if (SceneParser::isLightsCommand(cmd)) {
		return LIGHTS;
	}
	else if (SceneParser::isMaterialsCommand(cmd)) {
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
//void rightmultiply(const mat4 & M, stack<mat4> &transfstack)
//{
//	mat4 &T = transfstack.top();
//	T = T * M;
//}

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

	while (in) {

		getline(in, str);
		// Ignore comments and white spaces of any kind
		if ((str.find_first_of(" \t\r\n") == string::npos) || (str[0] == '#')) {
			//cout << str << endl;;
			continue;
		}

		cout << str << endl;

		stringstream s(str);
		s >> cmd;

		//		const string size          = "size";
		//			const string maxdepth      = "maxdepth";
		//			const string output        = "output";
		//			const string camera        = "camera";

		CommandType command = SceneParser::identifyCommand(cmd);
		switch (command) {
		case GENERAL:
			SceneParser::handleGeneralCommand(s, cmd);
			break;

		case CAMERA:
			SceneParser::handleCameraCommand(s, cmd);
			break;

		case GEOMETRY:
			SceneParser::handleGeometryCommand(s, cmd);
			break;

		case TRANSFORMATIONS:
			SceneParser::handleTransformationsCommand(s, cmd);
			break;

		case LIGHTS:
			SceneParser::handleLightsCommand(s, cmd);
			break;

		case MATERIALS:
			SceneParser::handleMaterialsCommand(s, cmd);
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

		SceneParser::readValues(s, 2, values);
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
	glm::vec3 eyeInit = glm::vec3(values[0], values[1], values[2]);
	glm::vec3 center  = glm::vec3(values[3], values[4], values[5]);
	glm::vec3 upInit  = glm::vec3(values[6], values[7], values[8]);
	//upinit = Transform::upvector(upinit, eyeinit);
	GLfloat fovy = values[9];
	renderInfo.camera = new Camera(eyeInit, center, upInit, fovy, renderInfo.width, renderInfo.height);
}

void
SceneParser::handleGeometryCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.sphere) {
		readValues(s, 4, values);
		vec3 center = glm::vec3(values[0], values[1], values[2]);
		GLfloat radius = values[3];
		Object *sphere = new Sphere(center, radius, ambient);
		renderInfo.scene.addObject(sphere);
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
		Object *triangle = new Triangle(renderInfo.vertcies[values[0]],
				renderInfo.vertcies[values[1]],
				renderInfo.vertcies[values[2]],
				ambient);
		renderInfo.scene.addObject(triangle);
	}

	else if (cmd == Commands.trinormal) {
		readValues(s, 3, values);
		Object *triangle = new Triangle(renderInfo.vertcies[values[0] * 2],
				renderInfo.vertcies[values[1] * 2],
				renderInfo.vertcies[values[2] * 2],
				renderInfo.vertcies[values[0] * 2 - 1],
				renderInfo.vertcies[values[1] * 2 - 1],
				renderInfo.vertcies[values[2] * 2 - 1],
				ambient);
		renderInfo.scene.addObject(triangle);
	}
}
void SceneParser::handleTransformationsCommand(stringstream& s, string& cmd)
{

}
void SceneParser::handleLightsCommand(stringstream& s, string& cmd)
{

}
void SceneParser::handleMaterialsCommand(stringstream& s, string& cmd)
{
	if (cmd == Commands.ambient) {
		readValues(s, 3, values);
		ambient = vec3(values[0], values[1], values[2]);
	}
}





	/*

		throw IOException


		// I need to implement a matrix stack to store transforms.
		// This is done using standard STL Templates
		stack <mat4> transfstack;
	transfstack.push(mat4(1.0));  // identity

	getline (in, str);
	while (in) {
		if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
			// Ruled out comment and blank lines

			stringstream s(str);
			s >> cmd;
			int i;
			GLfloat values[10]; // Position and color for light, colors for others
			// Up to 10 params for cameras.
			bool validinput; // Validity of input

			// Process the light, add it to database.
			// Lighting Command
			if (cmd == "light") {
				if (numused == numLights) { // No more Lights
					cerr << "Reached Maximum Number of Lights " << numused << " Will ignore further lights\n";
				} else {
					validinput = readvals(s, 8, values); // Position/color for lts.
					if (validinput) {

						lightposn[4 * numused + 0] = values[0];
						lightposn[4 * numused + 1] = values[1];
						lightposn[4 * numused + 2] = values[2];
						lightposn[4 * numused + 3] = values[3];

						lightcolor[4 * numused + 0] = values[4];
						lightcolor[4 * numused + 1] = values[5];
						lightcolor[4 * numused + 2] = values[6];
						lightcolor[4 * numused + 3] = values[7];

						++numused;
					}
				}
			}

			// Material Commands
			// Ambient, diffuse, specular, shininess properties for each object.
			// Filling this in is pretty straightforward, so I've left it in
			// the skeleton, also as a hint of how to do the more complex ones.
			// Note that no transforms/stacks are applied to the colors.

			else if (cmd == "ambient") {
				validinput = readvals(s, 4, values); // colors
				if (validinput) {
					for (i = 0; i < 4; i++) {
						ambient[i] = values[i];
					}
				}
			} else if (cmd == "diffuse") {
				validinput = readvals(s, 4, values);
				if (validinput) {
					for (i = 0; i < 4; i++) {
						diffuse[i] = values[i];
					}
				}
			} else if (cmd == "specular") {
				validinput = readvals(s, 4, values);
				if (validinput) {
					for (i = 0; i < 4; i++) {
						specular[i] = values[i];
					}
				}
			} else if (cmd == "emission") {
				validinput = readvals(s, 4, values);
				if (validinput) {
					for (i = 0; i < 4; i++) {
						emission[i] = values[i];
					}
				}
			} else if (cmd == "shininess") {
				validinput = readvals(s, 1, values);
				if (validinput) {
					shininess = values[0];
				}
			} else if (cmd == "size") {
				validinput = readvals(s,2,values);
				if (validinput) {
					w = (int) values[0]; h = (int) values[1];
				}
			} else if (cmd == "camera") {
				validinput = readvals(s,10,values); // 10 values eye cen up fov
				if (validinput) {

					eyeinit = glm::vec3(values[0], values[1], values[2]);
					center  = glm::vec3(values[3], values[4], values[5]);
					upinit  = glm::vec3(values[6], values[7], values[8]);
					upinit = Transform::upvector(upinit, eyeinit);
					fovy = values[9];
				}
			}

			// I've left the code for loading objects in the skeleton, so
			// you can get a sense of how this works.
			// Also look at demo.txt to get a sense of why things are done this way.
			else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
				if (numobjects == maxobjects) { // No more objects
					cerr << "Reached Maximum Number of Objects " << numobjects << " Will ignore further objects\n";
				} else {
					validinput = readvals(s, 1, values);
					if (validinput) {
						object * obj = &(objects[numobjects]);
						obj->size = values[0];

						// Set the object's light properties
						for (i = 0; i < 4; i++) {
							(obj->ambient)[i] = ambient[i];
							(obj->diffuse)[i] = diffuse[i];
							(obj->specular)[i] = specular[i];
							(obj->emission)[i] = emission[i];
						}
						obj->shininess = shininess;

						// Set the object's transform
						obj->transform = transfstack.top();

						// Set the object's type
						if (cmd == "sphere") {
							obj->type = sphere;
						} else if (cmd == "cube") {
							obj->type = cube;
						} else if (cmd == "teapot") {
							obj->type = teapot;
						}
					}
					++numobjects;
				}
			}

			else if (cmd == "translate") {
				validinput = readvals(s,3,values);
				if (validinput) {

					rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
				}
			}
			else if (cmd == "scale") {
				validinput = readvals(s,3,values);
				if (validinput) {

					rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
				}
			}
			else if (cmd == "rotate") {
				validinput = readvals(s,4,values);
				if (validinput) {

					glm::mat3x3 rotate3x3 = Transform::rotate(values[3], glm::vec3(values[0], values[1], values[2]));
					glm::mat4x4 rotate4x4 = glm::mat4x4(vec4(rotate3x3[0], 0.0f),
							vec4(rotate3x3[1], 0.0f),
							vec4(rotate3x3[2], 0.0f),
							vec4(0.0f, 0.0f, 0.0f, 1.0f));
					rightmultiply(glm::transpose(rotate4x4), transfstack);
				}
			}

			// I include the basic push/pop code for matrix stacks
			else if (cmd == "pushTransform") {
				transfstack.push(transfstack.top());
			} else if (cmd == "popTransform") {
				if (transfstack.size() <= 1) {
					cerr << "Stack has no elements.  Cannot Pop\n";
				} else {
					transfstack.pop();
				}
			}

			else {
				cerr << "Unknown Command: " << cmd << " Skipping \n";
			}
		}

		getline (in, str);
	}

	eye = eyeinit;
	up = upinit;
	amount = amountinit;
	sx = sy = 1.0;  // keyboard controlled scales in x and y
	tx = ty = 0.0;  // keyboard controllled translation in x and y
	useGlu = false; // don't use the glu perspective/lookat fns

	glEnable(GL_DEPTH_TEST);
} else {
	cerr << "Unable to Open Input Data File " << filename << "\n";
	throw 2;
}
}

	 */


