/*
 * component.cpp
 *
 *  Created on: Jul 25, 2019
 *      Author: Ronn Quijada
 */

#include "component.h"

Component::Component(string name) {
	typeID = componentTypeID::basic;
	parent = NULL;
	this->name = name;
}

Component::~Component() {
}

void Component::initialize(Entity* parent) {
	this->parent = parent;
}

void Component::update() {
}

void Component::remove() {
	parent = NULL;
	name = "";
	typeID = componentTypeID::basic;
}

componentTypeID Component::getComponentTypeID() {
	return typeID;
}

string Component::getName() {
	return name;
}
