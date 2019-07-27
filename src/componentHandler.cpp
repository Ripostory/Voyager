/*
 * componentHandler.cpp
 *
 *  Created on: Jul 26, 2019
 *      Author: nog
 */

#include "componentHandler.h"

ComponentHandler::ComponentHandler() {
	parent = NULL;
}

ComponentHandler::~ComponentHandler() {
	parent = NULL;
	clearComponents();
}

void ComponentHandler::clearComponents() {
	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->remove();
		delete (*it);
		(*it) = NULL;
	}
}

void ComponentHandler::addComponent(Component* component) {
	components.push_back(component);
	//TODO sort by presedence
}

void ComponentHandler::initializeComponents() {
	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->initialize(parent);
	}
}

void ComponentHandler::updateComponents() {
	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->update();
	}
}

void ComponentHandler::setParent(Entity* parent) {
	this->parent = parent;
}
