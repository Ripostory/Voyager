
#ifndef INCLUDE_FRAMEBUFFER_H_
#define INCLUDE_FRAMEBUFFER_H_

#include "graphics_headers.h"
#include <vector>

struct RenderBuffer {
	GLuint RBO;
	GLenum attachment;
	bool isTexture;
	RenderBuffer(GLuint r, GLenum a, bool t): RBO(r), attachment(a), isTexture(t) {}
};

class FrameBuffer {
public:
	FrameBuffer();
	FrameBuffer(int width, int height);
	~FrameBuffer();

	void initFB(int width, int height);
    void addRBO(GLenum type, GLenum attach);
    void addRBOTex(GLenum type, GLenum attach);
    void bindFB();
    void bindRBTexture(int index, int attach);
    void bindDepth(int where);
    void bindAllTex();

    GLenum* getBufferList();
    int getBufferCount();
    int getWidth();
    int getHeight();
private:
    int width;
    int height;

    GLuint frameBuffer;
    GLuint depth;
    std::vector<RenderBuffer> renderBuffers;
    std::vector<GLenum> attachmentList;
};

#endif /* INCLUDE_FRAMEBUFFER_H_ */
