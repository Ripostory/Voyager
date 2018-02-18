/*
 * generator.cpp
 *
 *  Created on: Feb 17, 2018
 *      Author: Ronn Quijada
 */

#include "generator.h"

Generator::Generator(int width, int height)
{
	loadNewModel("assets/models/quad.obj");
	targetBuffer = NULL;
	generator = NULL;
	screenWidth = width;
	screenHeight = height;
}

Generator::~Generator()
{
}

void Generator::setTarget(FrameBuffer* newTarget)
{
	targetBuffer = newTarget;
}

void Generator::setGenerator(Shader* newGen)
{
	generator = newGen;
}

void Generator::generate()
{
	//bind framebuffer and draw to screen
	if (targetBuffer == NULL)
	{
		//if buffer is null, write to default buffer
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glViewport(0,0,screenWidth,screenHeight);
	}
	else
	{
		targetBuffer->bindFB();
		//switch to viewport
		glViewport(0,0,targetBuffer->getWidth(),targetBuffer->getHeight());
	}

	if (generator != NULL)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		generator->Enable();

		//pass in values to shader
		glUniform1i(generator->GetUniformLocation("screenWidth"), screenWidth);

		Object::Render();
	}
	else
	{
		cout << "ERROR: SHADER NOT ASSIGNED TO GENERATOR" << endl;
	}

	//reset viewport
	glViewport(0,0,screenWidth,screenHeight);
}



