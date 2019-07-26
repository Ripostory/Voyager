/*
 * component.h
 *
 *  Created on: Jul 25, 2019
 *      Author: Ronn Quijada
 *
 *  implements the most basic unit contained within an entity
 */

#ifndef INCLUDE_COMPONENT_H_
#define INCLUDE_COMPONENT_H_

#include "entity.h"
#include <string>

using namespace std;

enum componentTypeID {
	basic
};

class Entity;

class Component {
private:
	componentTypeID typeID;
	string name;
	Entity* parent;
public:
	Component(string name);
	virtual ~Component();
	virtual void initialize(Entity* parent);
	virtual void update();
	virtual void remove();
	componentTypeID getComponentTypeID();
	string getName();
};



#endif /* INCLUDE_COMPONENT_H_ */
