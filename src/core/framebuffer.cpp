#include <stdlib.h>
#include <string.h>

#include "core/common.h"
#include "core/framebuffer.h"

namespace pixelpipe {

FrameBuffer::FrameBuffer(int newNx, int newNy) : m_width(newNx), m_height(newNy)
{
	m_bAllocated = false;
}

FrameBuffer::~FrameBuffer()
{
	free(this->m_cData);
	free(this->m_zData);
}

void FrameBuffer::init()
{
	size_t c_len = 3 * m_width * m_height * sizeof(char);
	size_t z_len = m_width * m_height * sizeof(float);
	m_cData = (char*) malloc(c_len);
	m_zData = (float*) malloc(z_len);
	memset(m_cData, 0, c_len);
	memset(m_zData, 0, z_len);
	
	allocateGLTexture();
}

float FrameBuffer::getZ(const int x, const int y) const
{
	return this->m_zData[x + this->m_width * y];
}

void FrameBuffer::set(int ix, int iy, float r, float g, float b, float z)
{
	int offset = 3 * (ix + this->m_width * iy);

	this->m_cData[offset + 0] = (char) ((int) (255 * r) & 0xff);
	this->m_cData[offset + 1] = (char) ((int) (255 * g) & 0xff);
	this->m_cData[offset + 2] = (char) ((int) (255 * b) & 0xff);

	this->m_zData[ix + this->m_width * iy] = z;
}

void FrameBuffer::clear(float r, float g, float b, float z)
{
	char ir = (char) ((int) (255 * r) & 0xff);
	char ig = (char) ((int) (255 * g) & 0xff);
	char ib = (char) ((int) (255 * b) & 0xff);

	for (int k = 0; k < this->m_width * this->m_height; k++) {
		this->m_cData[3 * k + 0] = ir;
		this->m_cData[3 * k + 1] = ig;
		this->m_cData[3 * k + 2] = ib;
		this->m_zData[k] = z;
	}
}

void FrameBuffer::write(std::string filename)
{
	// IMG_PNG=1, IMG_TIFF=2, IMG_JPEG=3
	// write_image(const char *filename, const ByteRaster&, int type=-1);
	// try {
	//   FileOutputStream stream = new FileOutputStream(filename);
	//   String hdr = new String("P6 " + width + " " + height + " 255\n");
	//   stream.write(hdr.getBytes());
	//   for (int iy = height - 1; iy >= 0; iy--)
	//     stream.write(cData, 3 * width * iy, 3 * width);
	// }
	// catch (java.io.FileNotFoundException e) {
	//   throw new RuntimeException(e);
	// }
	// catch (java.io.IOException e) {
	//   throw new RuntimeException(e);
	// }
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

void FrameBuffer::drawGLTexture(float x, float y) const 
{
	//int step = img->widthStep/sizeof(char);
	/*
	for(int i=0; i<height; i++){
		for(int j=0; j<width; j++){
			for(int k=0; k<3; k++){
				tmp_img[(i*width + j)*channels + k] = ((char*)(data + i*step))[j*channels + k];
			}
		}
	}
	*/

	// Draw the texture using OpenGL
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);//(const_cast<GLuint*>(&textureHandle)));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, this->m_width, this->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->m_cData);
	
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(x, y, 0);
	glTexCoord2f(1, 0); glVertex3f(x + this->m_width, y, 0);
	glTexCoord2f(1, 1); glVertex3f(x + this->m_width, y + this->m_height, 0);
	glTexCoord2f(0, 1); glVertex3f(x, y + this->m_height, 0);
	glEnd();
		
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

}
