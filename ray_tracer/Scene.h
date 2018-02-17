/*
 * Scene.h
 *
 *  Created on: Jan 18, 2018
 *      Author: alonbs
 */

#ifndef SCENE_H_
#define SCENE_H_

#include <vector>
#include "Object.h"
#include "Lights.h"




class Scene {

private:

	std::vector<Object*> 	  		objects;
	std::vector<PointLight*>  		pointLights;
	std::vector<DirectionalLight*>  directionalLights;

	// Attenuation values

	Attenuation_t attenuation;

	int a;

public:

	Scene();
	virtual ~Scene();

	void addObject(Object *obj) { objects.push_back(obj); }
	void addPointLight(PointLight *light) { pointLights.push_back(light); }
	void addDirectionalLight(DirectionalLight *light) { directionalLights.push_back(light); }

	//void setAttenuation(Attenuation& atten) { this->attenuation = atten; }


	std::vector<Object*>& getObjects() { return objects; }
	std::vector<PointLight*>& getPointLights() { return pointLights; }
	std::vector<DirectionalLight*>& getDirectionalLights() { return directionalLights; }

	Attenuation_t& Attenuation() { return attenuation; }

	int& A() { return a; }

};

#endif /* SCENE_H_ */
