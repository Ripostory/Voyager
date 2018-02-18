
#include "framebuffer.h"

FrameBuffer::FrameBuffer()
{
	width = 0;
	height = 0;
	frameBuffer = -1;
	depth = -1;
}

FrameBuffer::FrameBuffer(int w, int h)
{
	initFB(w,h);
}

FrameBuffer::~FrameBuffer()
{
	width = 0;
	height = 0;
	frameBuffer = 0;

	//clean up everything
	glDeleteFramebuffers(1, &frameBuffer);
}

void FrameBuffer::initFB(int w, int h)
{
	width = w;
	height = h;

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void FrameBuffer::addRBO(GLenum type, GLenum attach)
{
	GLuint newRBO;
	glGenRenderbuffers(1, &newRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, newRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, attach, GL_RENDERBUFFER, newRBO);

	//push into RenderBuffer list
	renderBuffers.push_back(RenderBuffer(newRBO, attach, false));

	//don't push regular RBOs, since they can't be sampled from

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		auto error = glGetError();
		std::cerr << "Framebuffer creation was not successful: ERROR " << error << std::endl;
	}
}

void FrameBuffer::addRBOTex(GLenum type, GLenum attach)
{
	//allocate texture
	GLuint newRBO;
	glGenTextures(1, &newRBO);
	glBindTexture(GL_TEXTURE_2D, newRBO);

	if (type != GL_DEPTH_COMPONENT)
	{
		if (type == GL_RGBA || type == GL_RGB8 || type == GL_RGB10 || GL_R11F_G11F_B10F)
			glTexImage2D(GL_TEXTURE_2D, 0, type,
					width, height, 0, GL_RGB,
					GL_UNSIGNED_BYTE, NULL);

		else if (type == GL_RG || type == GL_RG8)
			glTexImage2D(GL_TEXTURE_2D, 0, type,
					width, height, 0, GL_RG,
					GL_UNSIGNED_BYTE, NULL);

		else if (type == GL_R || type == GL_R8)
			glTexImage2D(GL_TEXTURE_2D, 0, type,
					width, height, 0, type,
					GL_UNSIGNED_BYTE, NULL);

		else if (type == GL_RGBA16F || type == GL_RGBA32F || type == GL_RGB10_A2)
			glTexImage2D(GL_TEXTURE_2D, 0, type,
					width, height, 0, GL_RGBA,
					GL_FLOAT, NULL);

		else if (type == GL_RGB16F || type == GL_RGB32F || type == GL_RGB10_A2)
			glTexImage2D(GL_TEXTURE_2D, 0, type,
					width, height, 0, GL_RGB,
					GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		if (type == GL_DEPTH_COMPONENT)
			glTexImage2D(GL_TEXTURE_2D, 0, type,
					width, height, 0, GL_DEPTH_COMPONENT,
					GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}


	glFramebufferTexture2D(
	    GL_FRAMEBUFFER, attach, GL_TEXTURE_2D, newRBO, 0
	);

	if (type != GL_DEPTH_COMPONENT)
	{
		//push into RenderBuffer list
		renderBuffers.push_back(RenderBuffer(newRBO, attach, true));

		//add to attachmentList
		attachmentList.push_back(attach);
	}
	else
	{
		//put it into depth
		depth = newRBO;
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		auto error = glGetError();
		std::cerr << "Framebuffer creation was not successful: ERROR " << error << std::endl;
	}
}

void FrameBuffer::bindFB()
{
	//bind and enable draw buffers
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffers(getBufferCount(), getBufferList());
}

void FrameBuffer::bindRBTexture(int index, int attach)
{
	//bind specific texture
	glActiveTexture(attach);
	glBindTexture(GL_TEXTURE_2D, renderBuffers[index].RBO);
}

void FrameBuffer::bindDepth(int where)
{
	glActiveTexture(GL_TEXTURE0 + where);
	glBindTexture(GL_TEXTURE_2D, depth);
}

void FrameBuffer::bindAllTex()
{
	//bind all Texture bound render buffers
	//set their attachment based on index
	//NOTE: this works only with 2D textures

	for (int i = 0; i < getBufferCount(); i++)
	{
		bindRBTexture(i, GL_TEXTURE0 + i);
	}
}

GLenum* FrameBuffer::getBufferList()
{
	return &attachmentList[0];
}

int FrameBuffer::getBufferCount()
{
	return attachmentList.size();
}

int FrameBuffer::getWidth()
{
	return width;
}

int FrameBuffer::getHeight()
{
	return height;
}




