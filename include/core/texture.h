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
	 * Allocates an empty texture with the given parameters
	 * 
	 * @param width measured in pixels
	 * @param height measured in pixels
	 * @param channels channels (1=luminance|alpha, 3=rgb, 4=rgba, etc)
	 * @param format storage format for texture (float, int, etc)
	 */
	Texture(const unsigned width, const unsigned height, const unsigned channels = 4, pixel_type format = PIXEL_TYPE_UNSIGNED_BYTE);
	
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
		*(this->m_raster) = tex.getTextureData();
		// if(m_raster!=NULL) delete m_raster;
		// m_raster = new cg::image::ByteRaster(tex.getTextureData().width(), tex.getTextureData().height(), tex.getTextureData().channels());
		// *m_raster = tex.getTextureData();
		
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
	 * Resets the texture data to the data supplied within the parameters. 
	 * Any pre-existing data is deleted.
	 * 
	 * @param width the width of the new texture information
	 * @param height the height of the new texture information
	 * @param channels the number of channels to be used
	 * @param data the raw byte data of the new texture
	 */
	void setTextureData(const unsigned width, const unsigned height, const unsigned channels, const void* data);
	
	/**
	 * Accessor method for the ByteRaster object.
	 * 
	 * @return a const reference to the current Raster object.
	 */
	const cg::image::ByteRaster& getTextureData() const { return *m_raster; }
	
	/**
	 * Accessor method for the raw texture data.
	 * 
	 * @return a const reference to the current texture data.
	 */
	unsigned char* getTextureBytes() const { return m_raster->head(); }

	
protected:
	// std::string m_filename;				//!< The name of the file from where the data was loaded.
	cg::image::ByteRaster* m_raster;	//!< The RGB data for each pixel.

};	// class Texture

}	// namespace pixelpipe


/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::Texture& t)
{
	return out << "[ Texture: width=" << t.width() << ", height=" << t.height() << " ]";
}

#endif	// __PIPELINE_TEXTURE_H