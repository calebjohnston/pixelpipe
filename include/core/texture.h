#ifndef __PIPELINE_TEXTURE_H
#define __PIPELINE_TEXTURE_H

#include <string>

#include "cg/image/raster.h"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/color.h"

namespace pixelpipe {

/*!
 * \class Texture "core/texture.h"
 * \brief This class holds all of the information necessary to describe texture data.
 * 
 */
class Texture {
public:
	/**
	 * Default constructor. Does nothing.
	 */
	Texture();
	
	/**
	 * Reads in a texture from the given file and stores the data. 
	 * 
	 * @param imageFile The file where the texture is stored.
	 */
	Texture(std::string filename);
	
	/**
	 * Copy constructor.
	 */
	Texture(const Texture& tex)
	{
		*this = tex;
	}
	
	/**
	 * Copy constructor.
	 */
	Texture& operator=(const Texture& tex)
	{
		//*(this->cBuf) = tex.getTextureData();
		if(cBuf!=NULL) delete cBuf;
		cBuf = new cg::image::ByteRaster(tex.getTextureData().width(), tex.getTextureData().height(), tex.getTextureData().channels());
		*cBuf = tex.getTextureData();
		
		return *this;
	}
	
	/**
	 * Destructor.
	 */
	~Texture();

	/**
	 * Accessor method for the texture width.
	 */
	int width() const;
	
	/**
	 * Accessor method for the texture height;
	 */
	int height() const;
	
	/**
	 * Samples this texture for a given 2D location using linear interpolation. 
	 * Places the result in the color parameter.
	 * 
	 * @param p The 2D texture coordinate.
	 * @param cOut The result of sampling the texture.
	 */
	cg::vecmath::Color3f sample(const float u, const float v) const;
	
	/**
	 * Accessor method for updating the texture buffer data.
	 * 
	 * @param buffer the new raster buffer to use
	 */
	void setTextureData(const cg::image::ByteRaster& buffer);
	
	/**
	 * Accessor method for the ByteRaster object.
	 * 
	 * @return a const reference to the current Raster object.
	 */
	const cg::image::ByteRaster& getTextureData() const { return *cBuf; }
	
	/**
	 * Accessor method for the raw texture data.
	 * 
	 * @return a const reference to the current texture data.
	 */
	unsigned char* getTextureBytes() const { return cBuf->head(); }

	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Texture: width=" << width() << ", height=" << height() << " ]";
	}
	
protected:
	std::string filename;			//!< The name of the file from where the data was loaded.
	cg::image::ByteRaster* cBuf;	//!< The RGB data for each pixel.

};	// class Texture

}	// namespace pixelpipe

#endif	// __PIPELINE_TEXTURE_H