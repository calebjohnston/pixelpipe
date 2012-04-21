#include <stdlib.h>
#include <string.h>

#include "core/common.h"
#include "core/framebuffer.h"

namespace pixelpipe {

FrameBuffer::FrameBuffer(const unsigned width, const unsigned height, const unsigned channels) : Texture(width, height, channels)
{
	m_bAllocated = false;
}

FrameBuffer::~FrameBuffer()
{
	this->deallocateGLTexture();
}

void FrameBuffer::init()
{
	allocateGLTexture();
}

float FrameBuffer::getZ(const int x, const int y) const
{
	if(m_raster==NULL) return 0;
	
	// TODO: this requires bounds checking on the channels
	float* pixel = (*m_raster).pixel(x,y);
	float z = pixel[3];
	
	return z;
}

void FrameBuffer::set(int ix, int iy, float r, float g, float b, float z)
{
	if(m_raster==NULL) return;
	
	size_t offset = (iy * this->width() + ix) * 4;
	(*this->m_raster).at(offset) = r;
	(*this->m_raster).at(offset+1) = g;
	(*this->m_raster).at(offset+2) = b;
	(*this->m_raster).at(offset+3) = z;
}

void FrameBuffer::clear(float r, float g, float b, float z)
{
	if(m_raster==NULL) return;
	
	unsigned int w = this->width();
	unsigned int h = this->height();

	for (unsigned i = 0; i < this->height(); i++) {
		for (unsigned j = 0; j < this->width(); j++) {
			size_t offset = (i*w+j)*4;
			(*this->m_raster)[offset + 0] = r;
			(*this->m_raster)[offset + 1] = g;
			(*this->m_raster)[offset + 2] = b;
			(*this->m_raster)[offset + 3] = z;
		}
	}
}

void FrameBuffer::allocateGLTexture()
{	
	if(!this->m_bAllocated){
		glGenTextures(1, &(this->m_textureHandle));
		glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		this->m_bAllocated = true;
	}
}
void FrameBuffer::deallocateGLTexture()
{	
	if(!this->m_bAllocated){
		glDeleteTextures(1, &(this->m_textureHandle));
		this->m_bAllocated = false;
	}
}

void FrameBuffer::drawGLTexture(float x, float y) const 
{
	// Draw the texture using OpenGL
	unsigned w = this->width();
	unsigned h = this->height();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0, GL_RGBA, GL_FLOAT, this->m_raster->head());
	
	glBegin(GL_QUADS);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 1); glVertex3f(x, y, 0);
	glTexCoord2f(1, 1); glVertex3f(x + w, y, 0);
	glTexCoord2f(1, 0); glVertex3f(x + w, y + h, 0);
	glTexCoord2f(0, 0); glVertex3f(x, y + h, 0);
	glEnd();
	
	glDisable(GL_TEXTURE_2D);
}

}
