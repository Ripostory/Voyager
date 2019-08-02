/*
 * entity.h
 *
 *  Created on: Jul 25, 2019
 *      Author: Ronn Quijada
 *
 *  Implements the most basic unit of a 3D world
 */

#ifndef INCLUDE_ENTITY_H_
#define INCLUDE_ENTITY_H_

#include "graphics_headers.h"
#include "componentHandler.h"
#include <vector>

class Component;
class ComponentHandler;

class Entity {
private:
	glm::mat4 m_translate;
	glm::mat4 m_rotate;
	glm::mat4 m_scale;
	glm::mat4 model;
	void updateMatrix();
protected:
	ComponentHandler* components;
public:
	Entity();
	~Entity();
	void translate(glm::vec3 position);
	void scale(float scaleAmount);
	void rotate(float, glm::vec3 gimbal);
	glm::mat4 getModelMatrix();

	void initialize();
	void update();
	void addComponent(Component* component);
};

#endif /* INCLUDE_ENTITY_H_ */
