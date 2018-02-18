/*
 * planet.cpp
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#include "planet.h"

Planet::Planet()
{
	  loadNewModel("assets/models/planet.obj");
	  scale(7.0f);
}

Planet::~Planet()
{
}

void Planet::Render()
{
	//render
	Object::Render();
}



