/*
 * componentHandler.cpp
 *
 *  Created on: Jul 26, 2019
 *      Author: Ronn Quijada
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

bool ComponentHandler::componentSorter(Component* lhs,Component*  rhs) {
	return (lhs->getComponentTypeID() > rhs->getComponentTypeID());
}

void ComponentHandler::addComponent(Component* component) {
	components.push_back(component);
	sort(components.begin(), components.end(), componentSorter);
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

void ComponentHandler::renderComponents() {
	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->render();
	}
}

void ComponentHandler::setParent(Entity* parent) {
	this->parent = parent;
}
