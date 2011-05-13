#ifndef __PIPELINE_TEXTURE_H
#define __PIPELINE_TEXTURE_H

#include <string>

#include "cg/image/raster.h"
#include "cg/vecmath/vec3.h"
#include "cg/vecmath/color.h"

namespace pipeline {

/**
 * This class holds all of the information necessary to describe texture data.
 * 
 * @author ags
 */
class Texture {
public:
	Texture(std::string filename);
	~Texture();
	
	int width() const;
	int height() const;
	void sample(const float s, const float t, cg::vecmath::Color3f* cOut) const;	// TO-DO: THERE IS NO Color3f type. see vecmath/color.h
	
protected:
	std::string filename;	// The name of the file from where the data was loaded.

private:	
	cg::image::ByteRaster* cBuf;	// The RGB data for each pixel.
	// char[] cData;	// The RGB data for each pixel.

};	// class Texture

}	// namespace pipeline

#endif	// __PIPELINE_TEXTURE_H