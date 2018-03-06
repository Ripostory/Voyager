/*
 * planet.h
 *
 *  Created on: Sep 4, 2017
 *      Author: Ronn Quijada
 */

#ifndef PLANET_H_
#define PLANET_H_

#include "object.h"
#include "shader.h"
#include <math.h>

class Planet : public Object
{
public:
	Planet();
	~Planet();
	void Render(Shader*);
	void Update(unsigned int);
	void generate();
	void generateForeground();
	void generateChildren();
	void generateLight();
	void genGas();
	void genRocky();
	void genMoon();
	float randFloat();
	float seededRand();
	glm::vec3 randVec3();
	static glm::vec3 complement(glm::vec3);
	static glm::vec3 warmer(glm::vec3, float mag);
	static glm::vec3 cooler(glm::vec3, float mag);
	static glm::vec3 desaturate(glm::vec3, float);
	void frame();
	void place();
	void setSeed(float);

private:
	glm::vec3 color1;
	glm::vec3 color2;
	glm::vec3 color3;
	glm::vec3 color4;
	glm::vec3 atmosphere;
	glm::vec3 horizon;

	glm::vec3 lightPos;
	glm::vec2 distort;

	glm::vec3 childPos;
	float childRad;

	float seed;
	float roll;
	bool isGiant;
};



#endif /* PLANET_H_ */
