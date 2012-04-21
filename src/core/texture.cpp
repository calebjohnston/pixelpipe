#include <string>
#include <math.h>

#include "core/common.h"
#include "core/texture.h"

namespace pixelpipe {
	
using namespace cg::vecmath;
using namespace cg::image;

Texture::Texture()
{
	m_raster = NULL;
}

// TODO: Not supporting the last parameter yet!
Texture::Texture(const unsigned width, const unsigned height, const unsigned channels, pixel_type format)
{
	/*
	switch(format){
		case PIXEL_TYPE_BYTE:
			m_raster = new Raster<char>(width, height, channels);
			break;
		case PIXEL_TYPE_UNSIGNED_BYTE:
			m_raster = new Raster<unsigned char>(width, height, channels);
			break;
		case PIXEL_TYPE_UNSIGNED_SHORT:
			m_raster = new Raster<unsigned short>(width, height, channels);
			break;
		case PIXEL_TYPE_SHORT:
			m_raster = new Raster<short>(width, height, channels);
			break;
		case PIXEL_TYPE_UNSIGNED_INT:
			m_raster = new Raster<unsigned int>(width, height, channels);
			break;
		case PIXEL_TYPE_INT:
			m_raster = new Raster<int>(width, height, channels);
			break;
		case PIXEL_TYPE_FLOAT:
			m_raster = new Raster<float>(width, height, channels);
			break;
	}
	*/
	m_raster = new FloatRaster(width, height, channels);
}

Texture::Texture(std::string filename)
{
	m_raster = NULL;
	//char nom[100];
	///char* nom = (char*)'/Users/Caleb/Development/OpenSource/pixelpipe/resources/textures/carbonite.png';
	ByteRaster* byte_image = cg::image::read_image(filename.c_str());	// only supports TIFF, JPEG, and PNG
	m_raster = new FloatRaster(*byte_image);
	// DEV() << "Texture::Texture(" << filename.c_str() << ")";

	//reinterpret_cast<const char*>( interleaved_view_get_raw_data(imageView) ) 
	//FloatRaster* img = read_png_image(filename.c_str());
	//DEV() << "loaded " << m_raster->width() << " x " << m_raster->height() << " texture with " << m_raster->channels() << " channels.";
}

Texture::~Texture()
{
	if(m_raster!=NULL) delete m_raster;
}

int Texture::width() const 
{
	return (m_raster!=NULL)? m_raster->width(): 0;
}

int Texture::height() const 
{
	return (m_raster!=NULL)? m_raster->height(): 0;
}

void Texture::setTextureData(const unsigned width, const unsigned height, const unsigned channels, const void* data)
{
	if(m_raster!=NULL){
		delete m_raster;
	}
	m_raster = new FloatRaster(width, height, channels, (float*) data);
}

void Texture::setTextureData(const cg::image::FloatRaster& buffer)
{
	if(m_raster!=NULL) delete m_raster;
	
	*m_raster = buffer;
}

// TODO: Re-implement this function, apparently this doesn't work
void Texture::write(std::string filename)
{
	if(m_raster==NULL) return;
	
	// IMG_PNG=1, IMG_TIFF=2, IMG_JPEG=3
	ByteRaster* byte_image = new ByteRaster(*m_raster);
	cg::image::write_image(filename.c_str(), *byte_image, IMG_PNG);
}

Color3f Texture::sample(const float u, const float v) const
{
	Color3f cOut;
	
	float s = fabs(u);
	float t = fabs(v);
	
	// TO-DO: Check for null input in cOut parameter...
	int nx = m_raster->width();
	int ny = m_raster->height();
	int channels = m_raster->channels();
	float iX = s * nx + 0.5;
	float iY = t * ny + 0.5;
	
	// compute interpolation weights
	int x = (int) std::floor(iX);
	int y = (int) std::floor(iY);
	float uRatio = iX - x;
	float vRatio = iY - y;
	float uWeighted = 1.0 - uRatio;
	float vWeighted = 1.0 - vRatio;
	
	Color3f leftTop;
	Color3f rightTop;
	Color3f leftBottom;
	Color3f rightBottom;
	
	// store color of first corner
	int offset = channels * (nx * y + x);
	if(offset >= m_raster->length()) offset = m_raster->length() - channels;
	leftTop.set(m_raster->at(offset + 0), m_raster->at(offset + 1), m_raster->at(offset + 2));
	leftTop *= uWeighted;
	
	// store color of second corner
	offset = channels * (nx * y + x + 1);
	if(offset >= m_raster->length()) offset = m_raster->length() - channels;
	rightTop.set(m_raster->at(offset + 0), m_raster->at(offset + 1), m_raster->at(offset + 2));
	rightTop *= uRatio;
	
	// apply interpolation
	rightTop += leftTop;
	rightTop *= vWeighted;
	
	// store color of third corner
	offset = channels * (nx * (y+1) + x);
	if(offset >= m_raster->length()) offset = m_raster->length() - channels;
	leftBottom.set(m_raster->at(offset + 0), m_raster->at(offset + 1), m_raster->at(offset + 2));
	leftBottom *= uWeighted;
	
	// store color of fourth corner
	offset = channels * (nx * (y+1) + x + 1);
	if(offset >= m_raster->length()) offset = m_raster->length() - channels;
	rightBottom.set(m_raster->at(offset + 0), m_raster->at(offset + 1), m_raster->at(offset + 2));
	rightBottom *= uRatio;
	
	// apply interpolation
	rightBottom += leftBottom;
	rightBottom *= vRatio;
	
	cOut = rightBottom + rightTop;
	
	return cOut;
}

}	// namespace pixelpipe