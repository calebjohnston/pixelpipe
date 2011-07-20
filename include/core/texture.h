#ifndef __PIPELINE_TEXTURE_H
#define __PIPELINE_TEXTURE_H

#include <string>

#include "cg/image/raster.h"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/color.h"

namespace pipeline {

/**
 * This class holds all of the information necessary to describe texture data.
 * 
 * @author ags
 */
class Texture {
public:
	// constructors
	// 
	Texture();
	Texture(std::string filename);
	
	// copy constructors
	// 
	Texture(const Texture& tex)
	{
		*this = tex;
	}
	
	Texture& operator=(const Texture& tex)
	{
		*(this->cBuf) = tex.getTextureData();
		
		return *this;
	}
	
	~Texture();
	
	int width() const;
	int height() const;
	cg::vecmath::Color3f sample(const float s, const float t) const;	// TO-DO: THERE IS NO Color3f type. see vecmath/color.h
	void setTextureData(const cg::image::ByteRaster& buffer);
	const cg::image::ByteRaster& getTextureData() const { return *cBuf; }
	
protected:
	std::string filename;	// The name of the file from where the data was loaded.
	cg::image::ByteRaster* cBuf;	// The RGB data for each pixel.

private:
	// char[] cData;	// The RGB data for each pixel.

};	// class Texture

}	// namespace pipeline

#endif	// __PIPELINE_TEXTURE_H