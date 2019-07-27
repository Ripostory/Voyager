/*
 * componentHandler.h
 *
 *  Created on: Jul 26, 2019
 *      Author: Ronn Quijada
 */

#ifndef INCLUDE_COMPONENTHANDLER_H_
#define INCLUDE_COMPONENTHANDLER_H_

#include "component.h"
#include "entity.h"
#include <vector>
#include <algorithm>

class Entity;
class Component;

class ComponentHandler {
private:
	Entity* parent;
	vector<Component*> components;
	static bool componentSorter(Component* lhs,Component*  rhs);
public:
	ComponentHandler();
	~ComponentHandler();
	void clearComponents();
	void addComponent(Component* component);
	void initializeComponents();
	void updateComponents();
	void setParent(Entity* parent);
};


#endif /* INCLUDE_COMPONENTHANDLER_H_ */
