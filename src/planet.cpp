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
	  seed = 20;
	  roll = 0;
	  childRad = 0;
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
	//modify seed to be between 0 and 1;
	//older versions of openGL don't like having a large seed
	float boundedSeed;
	boundedSeed = (seed + 123.5453)/(123.5453*2.0);

	//pass in seed
	glUniform1f(ref->GetUniformLocation("inpSeed"),boundedSeed);

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

	//pass in planet center
	glm::vec3 position;
	position.x = model[3][0];
	position.y = model[3][1];
	position.z = model[3][2];
	float radius = model[0][0];
	glm::mat4 rollRotate;
	rollRotate = glm::rotate(roll*10.0f, glm::vec3(0,0,1));
	glUniform3fv(ref->GetUniformLocation("center"),1, glm::value_ptr(position));
	glUniform1f(ref->GetUniformLocation("radius"), radius);
	glUniformMatrix4fv(ref->GetUniformLocation("roll"),1, false, glm::value_ptr(rollRotate));

	//pass in child planet coordinates
	glUniform3fv(ref->GetUniformLocation("childPos"),1, glm::value_ptr(childPos));
	glUniform1f(ref->GetUniformLocation("childRad"), childRad);

	//render
	Object::Render();
}

void Planet::Update(unsigned int dt)
{
}

void Planet::generate()
{
	if (seededRand() < 0.5)
	{
		genGas();
	}
	else
	{
		genRocky();
	}
}

void Planet::generateForeground()
{
	//assume the light has already been generated
	generate();
	//frame planet
	frame();
}

void Planet::generateChildren()
{
	isGiant = false;
	generateLight();
	genMoon();
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
	color2 = warmer(complement(color1), seededRand());
	color3 = warmer(color1, seededRand());
	color4 = cooler(color2, seededRand());

	atmosphere = cooler((color1+color2)/2.0f, seededRand());
	horizon = warmer(complement(atmosphere), seededRand());
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
	color2 = warmer(color1, seededRand());
	color3 = randVec3();
	color4 = cooler(color3, seededRand());

	atmosphere = cooler((color1+color2)/2.0f, seededRand());
	horizon = warmer(atmosphere, seededRand());
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
	//generate based on seed
	color1 = desaturate(randVec3(), 0.5);
	color2 = warmer(color1, seededRand()/2.0f);
	color3 = warmer(color1, seededRand()/2.0f);
	color4 = cooler(color3, seededRand()/2.0f);

	atmosphere = cooler((color1+color2)/4.0f, seededRand());
	horizon = warmer(atmosphere/2.0f, seededRand());
}

void Planet::generateLight()
{
	//purely random
	lightPos.x = randFloat()*2.0 - 1.0;
	lightPos.y = randFloat()*2.0 - 1.0;
	lightPos.z = -(randFloat()*2.0 - 0.5);
}

float Planet::randFloat()
{
	return ((float) rand()/RAND_MAX + seededRand())/2.0f;
}

float Planet::seededRand()
{
	//generate random
	seed = (glm::sin(seed*332312.553) * 123.5453);
	return ((seed/123.5453)+1.0)/2.0;
}

glm::vec3 Planet::randVec3()
{
	return glm::vec3(seededRand(),seededRand(),seededRand());
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

	glm::vec3 move = glm::vec3(4*-light.x, 5*-light.y ,0);
	move.x += (randFloat()-0.5f)*4.0;
	move.y += (randFloat()-0.5f)*4.0;

	//random roll
	roll = randFloat();

	//do a roll to determine if we want to do extreme zoom in
	if (randFloat() < 0.3f)
	{
		//do extreme zoom in
		scale((randFloat()*5.0f)+10.0f);
		translate(move*2.0f + glm::vec3(0,0,randFloat()*-30.0f));
	}
	else
	{
		translate(move);
	}
}

void Planet::place()
{
	//place in relation to parent
	float zPlace = -((randFloat()*80.0f)+20.0f);

	//reside in a random area
	glm::vec3 move = glm::vec3(0,0,zPlace);
	move.x += (randFloat()-0.5f)*8.0*(zPlace/20.0f);
	move.y += (randFloat()-0.5f)*8.0*(zPlace/20.0f);

	translate(move);

	childPos = move;
	childRad = (randFloat()*0.8f)+0.1f;
	//make smaller
	scale(childRad);

}

void Planet::setSeed(float newseed)
{
	seed = newseed;

	//put seed within bounds
	seededRand();

	//seed randomizer
	int counter = fmod(100, seed);
	for (int i = 0; i < counter; i++)
	{
		rand();
	}
}





