/*
 * entity.cpp
 *
 *  Created on: Jul 25, 2019
 *      Author: Ronn Quijada
 */

#include "entity.h"

Entity::Entity() {
	model = glm::mat4(1.0f);
	m_translate = glm::mat4(1.0f);
	m_rotate = glm::mat4(1.0f);
	m_scale = glm::mat4(1.0f);
}

Entity::~Entity() {
	model = glm::mat4(0.0f);
	m_translate = glm::mat4(0.0f);
	m_rotate = glm::mat4(0.0f);
	m_scale = glm::mat4(0.0f);

	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->remove();
	}
}

void Entity::initialize() {
	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->initialize(this);
	}
}

void Entity::update() {
	vector<Component*>::iterator it;
	for (it = components.begin(); it != components.end(); it++) {
		(*it)->update();
	}
}

void Entity::addComponent(Component* component)
{
	components.push_back(component);
}


void Entity::translate(glm::vec3 move)
{
	m_translate = glm::translate(move);
	updateMatrix();
}

void Entity::scale(float size)
{
	m_scale = glm::scale(glm::vec3(size));
	updateMatrix();
}

void Entity::rotate(float degree, glm::vec3 axis)
{
	m_rotate = glm::rotate(degree, axis);
	updateMatrix();
}

void Entity::updateMatrix()
{
	model = m_translate * m_scale * m_rotate;
}

glm::mat4 Entity::getModelMatrix()
{
  return model;
}

