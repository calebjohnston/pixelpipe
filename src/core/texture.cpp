#include <string>

#include "core/texture.h"

namespace pipeline {

Texture::Texture()
{
}

Texture::Texture(std::string filename)
{
	cBuf = cg::image::read_image(filename.c_str());	// only supports TIFF, JPEG, and PNG
	std::cout << "loaded " << cBuf->width() << " x " << cBuf->height() << " texture with " << cBuf->channels() << " channels." << std::endl;
}

Texture::~Texture()
{
	if(cBuf!=NULL) delete cBuf;
}

int Texture::width() const 
{
	int w = 0;
	if(cBuf!=NULL) w = cBuf->width();
	return w;
}

int Texture::height() const 
{
	int h = 0;
	if(cBuf!=NULL) h = cBuf->height();
	return h;
}

void Texture::setTextureData(const cg::image::ByteRaster& buffer)
{
	if(cBuf!=NULL) delete cBuf;
	
	*cBuf = buffer;
}

cg::vecmath::Color3f Texture::sample(const float s, const float t) const
{
	cg::vecmath::Color3f cOut;
	
	// TO-DO: Check for null input in cOut parameter...
	int nx = cBuf->width();
	int ny = cBuf->height();
	float iX = s * nx + 0.5f;
	float iY = t * ny + 0.5f;
	
	int x = (int) std::floor(iX);
	int y = (int) std::floor(iY);
	float uRatio = iX - x;
	float vRatio = iY - y;
	float uWeighted = 1.0f - uRatio;
	float vWeighted = 1.0f - vRatio;
	
	cg::vecmath::Color3f leftTop;
	cg::vecmath::Color3f rightTop;
	cg::vecmath::Color3f leftBottom;
	cg::vecmath::Color3f rightBottom;
	
	unsigned char r,g,b;
	
	int offset = 3 * (nx * y + x);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	r = cBuf->at(offset + 0);
	g = cBuf->at(offset + 1);
	b = cBuf->at(offset + 2);
	leftTop.set((r & 0xff) / 255.0f, (g & 0xff) / 255.0f, (b & 0xff) / 255.0f);
	leftTop *= uWeighted;
	
	offset = 3 * (nx * y + x + 1);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	rightTop.set((cBuf->at(offset + 0) & 0xff) / 255.0f, (cBuf->at(offset + 1) & 0xff) / 255.0f, (cBuf->at(offset + 2) & 0xff) / 255.0f);
	rightTop *= uRatio;
	
	rightTop += leftTop;
	rightTop *= vWeighted;
	
	offset = 3 * (nx * (y+1) + x);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	leftBottom.set((cBuf->at(offset + 0) & 0xff) / 255.0f, (cBuf->at(offset + 1) & 0xff) / 255.0f, (cBuf->at(offset + 2) & 0xff) / 255.0f);
	leftBottom *= uWeighted;
	
	offset = 3 * (nx * (y+1) + x + 1);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	rightBottom.set((cBuf->at(offset + 0) & 0xff) / 255.0f, (cBuf->at(offset + 1) & 0xff) / 255.0f, (cBuf->at(offset + 2) & 0xff) / 255.0f);
	rightBottom *= uRatio;
	
	rightBottom += leftBottom;
	rightBottom *= vRatio;
	
	cOut = rightBottom + rightTop;
	
	return cOut;
}

}	// namespace pipeline