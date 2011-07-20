#ifndef __PIPELINE_FRAMEBUFFER_H
#define __PIPELINE_FRAMEBUFFER_H

#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace pipeline {

/**
 * This class holds all of the data for a frame. A frame is usually constructed
 * or rendered off screen, then transferred onto the screen. This transfer
 * process is called blitting. The z buffer is also held in this frame buffer
 * class.
 * 
 */
class FrameBuffer {
public:
	FrameBuffer(int newNx, int newNy);
	~FrameBuffer();
	char* getData() const { return cData; }
	const int getWidth() const { return width; }
	const int getHeight() const { return height; }
	float getZ(const int x, const int y) const;
	void set(int ix, int iy, float r, float g, float b, float z);
	void clear(float r, float g, float b, float z);
	void write(std::string fname);
	void draw(float x=0, float y=0) const { this->drawGLTexture(x,y); };

protected:
	int width;	// The width of the image in the frame buffer.
	int height;	// The height of the image in the frame buffer.
	char* cData;	// The rgb data that forms the image.
	float* zData;	// The z buffer - holds the z value of the current fragment.
	
	void allocateGLTexture();
	void drawGLTexture(float x=0, float y=0) const;
	GLuint textureHandle;
	bool bAllocated;

};	// class FrameBuffer

}	// namespace pipeline

#endif	// __PIPELINE_FRAMEBUFFER_H