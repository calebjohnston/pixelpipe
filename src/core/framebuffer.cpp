#include <stdlib.h>
#include <string.h>

#include "core/framebuffer.h"

namespace pipeline {

/**
 * Constructs a new frame buffer with the given dimensions.
 * 
 * @param newNx The width of the new frame buffer.
 * @param newNy The height of the new frame buffer.
 */
FrameBuffer::FrameBuffer(int newNx, int newNy) : width(newNx), height(newNy)
{
	// cData = new byte[width * height * 3];
	// 	zData = new float[width * height];
	size_t c_len = 3 * width * height * sizeof(char);
	size_t z_len = width * height * sizeof(float);
	cData = (char*) malloc(c_len);
	zData = (float*) malloc(z_len);
	memset(cData, 0, c_len);
	memset(zData, 0, z_len);
	
	bAllocated = false;
	
	allocateGLTexture();
}

FrameBuffer::~FrameBuffer()
{
	free(cData);
	free(zData);
}

/**
 * Returns the z value of the currently stored fragment for the given (x, y)
 * coordinate.
 * 
 * @param x The x coordinate.
 * @param y The y coordinate.
 * @return The z value of the fragment stored at that point.
 */
float FrameBuffer::getZ(const int x, const int y) const
{
	return zData[x + width * y];
}

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
void FrameBuffer::set(int ix, int iy, float r, float g, float b, float z)
{
	int offset = 3 * (ix + width * iy);

	cData[offset + 0] = (char) ((int) (255 * r) & 0xff);
	cData[offset + 1] = (char) ((int) (255 * g) & 0xff);
	cData[offset + 2] = (char) ((int) (255 * b) & 0xff);

	zData[ix + width * iy] = z;
}

/**
 * Sets all data in the frame buffer to be the same color triple and depth
 * value.
 * 
 * @param r The red color channel (in the range [0, 1].
 * @param g The green color channel (in the range [0, 1].
 * @param b The blue color channel (in the range [0, 1].
 * @param z The new z value.
 */
void FrameBuffer::clear(float r, float g, float b, float z)
{
	char ir = (char) ((int) (255 * r) & 0xff);
	char ig = (char) ((int) (255 * g) & 0xff);
	char ib = (char) ((int) (255 * b) & 0xff);

	for (int k = 0; k < width * height; k++) {
		cData[3 * k + 0] = ir;
		cData[3 * k + 1] = ig;
		cData[3 * k + 2] = ib;
		zData[k] = z;
	}
}

/**
 * Attempts to write the current framebuffer out to a PPM file. In case of an
 * error, this method will throw a new RuntimeException.
 * 
 * @param fname The name of the output file.
 */
void FrameBuffer::write(std::string fname)
{
	// try {
	//   FileOutputStream stream = new FileOutputStream(fname);
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
	if(!bAllocated){
		glGenTextures(1, &textureHandle);
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		bAllocated = true;
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
	glBindTexture(GL_TEXTURE_2D, textureHandle);//(const_cast<GLuint*>(&textureHandle)));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, cData);
	
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glTexCoord2f(0, 0); glVertex3f(x, y, 0);
	glTexCoord2f(1, 0); glVertex3f(x + width, y, 0);
	glTexCoord2f(1, 1); glVertex3f(x + width, y + height, 0);
	glTexCoord2f(0, 1); glVertex3f(x, y + height, 0);
	glEnd();
		
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
}

}
