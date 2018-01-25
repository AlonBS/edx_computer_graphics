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



class Scene {

private:

	std::vector<Object*> objects;



public:
	Scene();
	virtual ~Scene();

	void addObject(Object *obj) { objects.push_back(obj); }

	std::vector<Object*>& getObjects() { return objects; }


};

#endif /* SCENE_H_ */
