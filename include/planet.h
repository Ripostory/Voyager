/*
 * planet.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "object.h"

class Planet : public Object
{
public:
	Planet();
	~Planet();
	void Render();

private:
	glm::vec3 color1;
	glm::vec3 color2;
	glm::vec3 color3;
	glm::vec3 color4;

	float seed;
};



#endif /* PLANET_H_ */
