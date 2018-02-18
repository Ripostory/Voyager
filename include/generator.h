/*
 * generator.h
 *
 *  Created on: Feb 17, 2018
 *      Author: Ronn Quijada
 */

#ifndef INCLUDE_GENERATOR_H_
#define INCLUDE_GENERATOR_H_

#include "object.h"
#include "framebuffer.h"
#include "shader.h"

class Generator : public Object
{
public:
	Generator(int width, int height);
	~Generator();
	void setTarget(FrameBuffer*);
	void setGenerator(Shader*);
	void generate();
private:
	FrameBuffer *targetBuffer;
	Shader *generator;
	int screenWidth;
	int screenHeight;
};



#endif /* INCLUDE_GENERATOR_H_ */
