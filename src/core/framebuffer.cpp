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
	// free(this->m_cData);
	// free(this->m_zData);
}

void FrameBuffer::init()
{
	/*
	size_t c_len = 3 * m_width * m_height * sizeof(char);
	size_t z_len = m_width * m_height * sizeof(float);
	m_cData = (char*) malloc(c_len);
	m_zData = (float*) malloc(z_len);
	memset(m_cData, 0, c_len);
	memset(m_zData, 0, z_len);
	*/
	
	allocateGLTexture();
}

float FrameBuffer::getZ(const int x, const int y) const
{
	// return this->m_zData[x + this->m_width * y];
	// return this->m_raster->head() + (y * this->m_raster->width() + x)*this->m_raster->channels();

	if(m_raster==NULL) return 0;
	
	unsigned char* pixel = (*m_raster).pixel(x,y);

	// TODO: this requires bounds checking on the channels
	float z = ((float)pixel[3]) / 255.0;
}

static int testing = 0;

void FrameBuffer::set(int ix, int iy, float r, float g, float b, float z)
{
	if(m_raster==NULL) return;
	
	size_t offset = (iy * this->width() + ix) * 4;
	(*this->m_raster).at(offset) = (unsigned char) (255 * r);
	(*this->m_raster).at(offset+1) = (unsigned char) (255 * g);
	(*this->m_raster).at(offset+2) = (unsigned char) (255 * b);
	// (*this->m_raster).at(offset+3) = (unsigned char) (255 * z);	// not good..
	
	/*
	int offset = 3 * (ix + this->m_width * iy);

	this->m_cData[offset + 0] = (char) ((int) (255 * r) & 0xff);
	this->m_cData[offset + 1] = (char) ((int) (255 * g) & 0xff);
	this->m_cData[offset + 2] = (char) ((int) (255 * b) & 0xff);

	this->m_zData[ix + this->m_width * iy] = z;
	*/
}

void FrameBuffer::clear(float r, float g, float b, float z)
{
	if(m_raster==NULL) return;
	
	unsigned int w = this->width();
	unsigned int h = this->height();
	unsigned char ir = (unsigned char) ((int) (255 * r) & 0xff);
	unsigned char ig = (unsigned char) ((int) (255 * g) & 0xff);
	unsigned char ib = (unsigned char) ((int) (255 * b) & 0xff);
	unsigned char iz = (unsigned char) ((int) (255 * z) & 0xff);

	for (int i = 0; i < this->height(); i++) {
		for (int j = 0; j < this->width(); j++) {
			size_t offset = (i*w+j)*4;
			(*this->m_raster)[offset + 0] = ir;
			(*this->m_raster)[offset + 1] = ig;
			(*this->m_raster)[offset + 2] = ib;
			(*this->m_raster)[offset + 3] = iz;
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->m_raster->head());
	
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
