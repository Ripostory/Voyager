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
	  seed = 0;
	  color1 = glm::vec3(0.8, 0.5, 0.1);
	  color2 = glm::vec3(0.4, 0.9, 0.7);
	  color3 = glm::vec3(0, 0.4, 0.5);
	  color4 = glm::vec3(1, 1, 1);
	  atmosphere = glm::vec3(0.3f, 0.5f, 0.7f);
	  horizon = glm::vec3(0.8f, 0.2f, 0.1f);
	  lightPos = glm::vec3(-50.0f, 1.0f, -50.0f);
	  distort = glm::vec2(0.2,1);
	  isGiant = false;
}

Planet::~Planet()
{
}

void Planet::Render(Shader *ref)
{
	//pass in seed
	glUniform1f(ref->GetUniformLocation("inpSeed"),seed);

	//pass in planet colors
	glUniform3fv(ref->GetUniformLocation("color1"),1, glm::value_ptr(color1));
	glUniform3fv(ref->GetUniformLocation("color2"),1, glm::value_ptr(color2));
	glUniform3fv(ref->GetUniformLocation("color3"),1, glm::value_ptr(color3));
	glUniform3fv(ref->GetUniformLocation("color4"),1, glm::value_ptr(color4));
	glUniform3fv(ref->GetUniformLocation("atmosphere"),1, glm::value_ptr(atmosphere));
	glUniform3fv(ref->GetUniformLocation("horizon"),1, glm::value_ptr(horizon));

	//pass in light
	glUniform3fv(ref->GetUniformLocation("lightPos"),1, glm::value_ptr(lightPos));

	//pass in warp (useful for generating gas giants)
	glUniform2fv(ref->GetUniformLocation("warp"),1, glm::value_ptr(distort));

	//render
	Object::Render();
}

void Planet::Update(unsigned int dt)
{
	generateForeground();
}

void Planet::generate()
{
	//choose to generate either rocky or gas giant
	switch (rand()%2)
	{
	case 0:
		genGas();
		break;
	case 1:
		genRocky();
		break;
	}
}

void Planet::generateForeground()
{
	//reset values
	scale(7.0f);
	color1 = glm::vec3(0.8, 0.5, 0.1);
	color2 = glm::vec3(0.4, 0.9, 0.7);
	color3 = glm::vec3(0, 0.4, 0.5);
	color4 = glm::vec3(1, 1, 1);
	atmosphere = glm::vec3(0.3f, 0.5f, 0.7f);
	horizon = glm::vec3(0.8f, 0.2f, 0.1f);
	lightPos = glm::vec3(-50.0f, 1.0f, -50.0f);
	distort = glm::vec2(0.2,1);
	isGiant = false;

	//create new seed
	seed += rand()*0.1f;
	seed = fmod(seed, 1000.0f);

	generate();
	generateLight();

	//frame planet
	frame();
}

void Planet::generateChildren()
{
	//create new seed
	seed += rand()*0.1f;
	seed = fmod(seed, 1000.0f);

	generate();
	//assume the light has already been generated
	//place into scene
	place();
}

void Planet::genGas()
{
	isGiant = true;
	scale((randFloat()*2.0f)+7.0f);
	//use distortion to create the gas giant 'banding'
	distort = glm::vec2(0.2,1);

	//choose more contrastig colors
	color1 = randVec3();
	color2 = warmer(complement(color1), randFloat());
	color3 = warmer(color1, randFloat());
	color4 = cooler(color2, randFloat());

	atmosphere = cooler((color1+color2)/2.0f, randFloat());
	horizon = warmer(complement(atmosphere), randFloat());
}

void Planet::genRocky()
{

	//shrink if there is a giant
	if (isGiant)
		scale(((randFloat()*1.0f)+7.0f)/2);
	else
		scale((randFloat()*1.0f)+7.0f);

	//use no distortion
	distort = glm::vec2(1,1);

	//choose random color
	color1 = randVec3();
	color2 = warmer(color1, randFloat());
	color3 = randVec3();
	color4 = cooler(color3, randFloat());

	atmosphere = cooler((color1+color2)/2.0f, randFloat());
	horizon = warmer(atmosphere, randFloat());
}

void Planet::genMoon()
{

	//lower detail
	distort = glm::vec2(0.1,0.1);

	//shrink if there is a giant
	if (isGiant)
		scale(((randFloat()*0.5f)+0.5f)/2);
	else
		scale((randFloat()*0.5f)+0.5f);

	//choose closer colors for more muted colors
	color1 = desaturate(randVec3(), 0.5);
	color2 = warmer(color1, randFloat()/2.0f);
	color3 = warmer(color1, randFloat()/2.0f);
	color4 = cooler(color3, randFloat()/2.0f);

	atmosphere = cooler((color1+color2)/4.0f, randFloat());
	horizon = warmer(atmosphere/2.0f, randFloat());
}

void Planet::generateLight()
{
	float angle = randFloat();
	lightPos.x = glm::sin((angle*2.0f)-1.0f)*50.0f;
	lightPos.y = (randFloat()*200.0f)-100.0f;
	lightPos.z = glm::cos(angle)*-50.0f;
}

float Planet::randFloat()
{
	return (float) rand()/RAND_MAX;
}

glm::vec3 Planet::randVec3()
{
	return glm::vec3(randFloat(),randFloat(),randFloat());
}

//color picking functions to choose more pleasing colors

glm::vec3 Planet::complement(glm::vec3 base)
{
	return glm::vec3(1.0f) - base;
}

glm::vec3 Planet::warmer(glm::vec3 base, float mag)
{
	glm::vec3 final;

	//lower base influence
	final = base;
	final *= 1.0 - mag;

	//shift to red and shift at a lower exponent to green
	final += glm::vec3(1,0.6,0)*mag;

	return final;
}

glm::vec3 Planet::cooler(glm::vec3 base, float mag)
{
	glm::vec3 final;

	//lower base influence
	final = base;
	final *= 1.0 - mag;

	//shift to blue and shift at a lower exponent to green
	final += glm::vec3(0,0.6,1)*mag;

	return final;
}

glm::vec3 Planet::desaturate(glm::vec3 color, float scale)
{
	float intensity = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;

	return (intensity*scale) + (color*(1-scale));
}

void Planet::frame()
{
	//frames the foreground parent

	//base location on light direction
	glm::vec3 light = glm::normalize(lightPos);

	glm::vec3 move = glm::vec3(10*-light.x, 3*-light.y ,0);
	move.x += (randFloat()-0.5f)*4.0;
	move.y += (randFloat()-0.5f)*4.0;

	//do a roll to determine if we want to do extreme zoom in
	if (randFloat() < 0.3f)
	{
		//do extreme zoom in
		scale((randFloat()*5.0f)+10.0f);
		translate(move/2.0f);
	}
	else
	{
		translate(move);
	}
}

void Planet::place()
{
	//place in relation to parent
	float zPlace = (randFloat()*80.0f)+50.0f;;

	//reside in a random area
	glm::vec3 move = glm::vec3(0,0,zPlace);
	move.x += (randFloat()-0.5f)*8.0*(zPlace/20.0f);
	move.y += (randFloat()-0.5f)*8.0*(zPlace/20.0f);

	translate(move);

}







