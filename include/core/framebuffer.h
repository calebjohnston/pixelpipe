#ifndef __PIPELINE_FRAMEBUFFER_H
#define __PIPELINE_FRAMEBUFFER_H

#include <string>
#include <iostream>

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

/*!
 * \class FrameBuffer "core/framebuffer.h"
 * \brief Simple class providing a render target interface for the Rasterizer.
 * 
 * This class holds all of the data for a frame. A frame is rendered (blit) off screen,  
 * then transferred onto the screen. The z buffer is also held in this frame buffer class.
 * 
 */
class FrameBuffer {
public:
	
	/**
	 * Constructs a new frame buffer with the given dimensions.
	 * 
	 * @param newNx The width of the new frame buffer.
	 * @param newNy The height of the new frame buffer.
	 */
	FrameBuffer(int newNx, int newNy);
	~FrameBuffer();
	char* getData() const { return cData; }
	const int getWidth() const { return width; }
	const int getHeight() const { return height; }
	
	/**
	 * Returns the z value of the currently stored fragment for the given (x, y)
	 * coordinate.
	 * 
	 * @param x The x coordinate.
	 * @param y The y coordinate.
	 * @return The z value of the fragment stored at that point.
	 */
	float getZ(const int x, const int y) const;
	
	/**
	 * Sets the color (r, g, b) and z value for a given (x, y) location.
	 * 
	 * @param ix The x coordinate.
	 * @param iy The y coordinate.
	 * @param r The red color channel (in the range [0, 1].
	 * @param g The green color channel (in the range [0, 1].
	 * @param b The blue color channel (in the range [0, 1].
	 * @param z The z value of the new fragment.
	 */
	void set(int ix, int iy, float r, float g, float b, float z);
	
	/**
	 * Sets all data in the frame buffer to be the same color triple and depth
	 * value.
	 * 
	 * @param r The red color channel (in the range [0, 1].
	 * @param g The green color channel (in the range [0, 1].
	 * @param b The blue color channel (in the range [0, 1].
	 * @param z The new z value.
	 */
	void clear(float r, float g, float b, float z);
	
	/**
	 * Attempts to write the current framebuffer out to a PNG file.
	 * 
	 * @param filename The name of the output file.
	 */
	void write(std::string filename);
	
	/**
	 * Draws this framebuffer by copying the buffer data to an OpenGL texture and 
	 * drawing the texture.
	 * 
	 * @param x the specified x location at which to start drawing
	 * @param x the specified y location at which to start drawing
	 */
	void draw(float x=0, float y=0) const { this->drawGLTexture(x,y); };
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ FrameBuffer ]";
	}

protected:
	int width;				//!< The width of the image in the frame buffer.
	int height;				//!< The height of the image in the frame buffer.
	char* cData;			//!< The rgb data that forms the image.
	float* zData;			//!< The z buffer - holds the z value of the current fragment.
	GLuint textureHandle;	//!< The OpenGL texture handle (used for drawing the framebuffer to the screen).
	bool bAllocated;		//!< The flag used for indicating whether or not the OpenGL texture was allocated yet.
	
	/**
	 * Allocates the texture object using OpenGL. Sets the bAllocated and textureHandle 
	 * values on success.
	 */
	void allocateGLTexture();
	
	/**
	 * Copies the frame buffer data to the allocated OpenGL texture and draws it.
	 * 
	 * @param x the specified x location at which to start drawing
	 * @param x the specified y location at which to start drawing
	 */
	void drawGLTexture(float x=0, float y=0) const;

};	// class FrameBuffer

}	// namespace pipeline

#endif	// __PIPELINE_FRAMEBUFFER_H