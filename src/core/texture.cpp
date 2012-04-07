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

Texture::Texture(std::string filename)
{
	m_raster = NULL;
	//char nom[100];
	///char* nom = (char*)'/Users/Caleb/Development/OpenSource/pixelpipe/resources/textures/carbonite.png';
	m_raster = cg::image::read_image(filename.c_str());	// only supports TIFF, JPEG, and PNG

	//reinterpret_cast<const char*>( interleaved_view_get_raw_data(imageView) ) 
	//ByteRaster* img = read_png_image(filename.c_str());
	//DEV() << "loaded " << m_raster->width() << " x " << m_raster->height() << " texture with " << m_raster->channels() << " channels.";
}

Texture::~Texture()
{
	if(m_raster!=NULL) delete m_raster;
}

int Texture::width() const 
{
	int w = 0;
	if(m_raster!=NULL) w = m_raster->width();
	return w;
}

int Texture::height() const 
{
	int h = 0;
	if(m_raster!=NULL) h = m_raster->height();
	return h;
}

void Texture::setTextureData(const cg::image::ByteRaster& buffer)
{
	if(m_raster!=NULL) delete m_raster;
	
	*m_raster = buffer;
}

Color3f Texture::sample(const float u, const float v) const
{
	Color3f cOut;
	
	float s = fabs(u);
	float t = fabs(v);
	
	// TO-DO: Check for null input in cOut parameter...
	int nx = m_raster->width();
	int ny = m_raster->height();
	float iX = s * nx + 0.5f;
	float iY = t * ny + 0.5f;
	
	int x = (int) std::floor(iX);
	int y = (int) std::floor(iY);
	float uRatio = iX - x;
	float vRatio = iY - y;
	float uWeighted = 1.0f - uRatio;
	float vWeighted = 1.0f - vRatio;
	// DEV() << "test nx=" << x <<", ny=" << y << ", s=" << s << ", t=" << t;
	
	Color3f leftTop;
	Color3f rightTop;
	Color3f leftBottom;
	Color3f rightBottom;
	
	unsigned char r,g,b;
	
	int offset = 3 * (nx * y + x);
	if(offset >= m_raster->length()) offset = m_raster->length() - 3;
	r = m_raster->at(offset + 0);
	g = m_raster->at(offset + 1);
	b = m_raster->at(offset + 2);
	leftTop.set((r & 0xff) / 255.0f, (g & 0xff) / 255.0f, (b & 0xff) / 255.0f);
	leftTop *= uWeighted;
	// DEV() << "test 1 offset=" << offset <<", length=" << m_raster->length();
	
	offset = 3 * (nx * y + x + 1);
	if(offset >= m_raster->length()) offset = m_raster->length() - 3;
	rightTop.set((m_raster->at(offset + 0) & 0xff) / 255.0f, (m_raster->at(offset + 1) & 0xff) / 255.0f, (m_raster->at(offset + 2) & 0xff) / 255.0f);
	rightTop *= uRatio;
	
	rightTop += leftTop;
	rightTop *= vWeighted;
	// DEV() << "test 2 offset=" << offset;
	
	offset = 3 * (nx * (y+1) + x);
	if(offset >= m_raster->length()) offset = m_raster->length() - 3;
	leftBottom.set((m_raster->at(offset + 0) & 0xff) / 255.0f, (m_raster->at(offset + 1) & 0xff) / 255.0f, (m_raster->at(offset + 2) & 0xff) / 255.0f);
	leftBottom *= uWeighted;
	// DEV() << "test 3 offset=" << offset;
	
	offset = 3 * (nx * (y+1) + x + 1);
	if(offset >= m_raster->length()) offset = m_raster->length() - 3;
	rightBottom.set((m_raster->at(offset + 0) & 0xff) / 255.0f, (m_raster->at(offset + 1) & 0xff) / 255.0f, (m_raster->at(offset + 2) & 0xff) / 255.0f);
	rightBottom *= uRatio;
	// DEV() << "test 4 offset=" << offset;
	
	rightBottom += leftBottom;
	rightBottom *= vRatio;
	
	cOut = rightBottom + rightTop;
	
	return cOut;
}

}	// namespace pixelpipe