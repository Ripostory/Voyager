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

class Entity {
private:
    glm::mat4 m_translate;
    glm::mat4 m_rotate;
    glm::mat4 m_scale;
    glm::mat4 model;
    void updateMatrix();
public:
    Entity();
    ~Entity();
    void translate(glm::vec3 position);
    void scale(float scaleAmount);
    void rotate(float, glm::vec3 gimbal);
    glm::mat4 getModel();
};



#endif /* INCLUDE_ENTITY_H_ */
