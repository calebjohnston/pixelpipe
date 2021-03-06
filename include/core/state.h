#ifndef __PIPELINE_STATE_H
#define __PIPELINE_STATE_H

#include <vector>

#include "core/common.h"
#include "core/pointlight.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"

namespace pixelpipe {

/*!
 * \class State "core/state.h"
 * \brief A class for storing the rendering state variables of the pipeline.
 * 
 * The rendering state is defined as all the configuration values for
 * Rasterization: lights, materials, framebuffer settings, etc
 * Geometry: points, normals, colors, texture coordinates, etc
 * Textures: texture maps, normal maps, texture filtering, etc
 * Matrices: user specified matrix stacks, texture matrix, etc
 * 
 */
class State {
public:	
	/**
	 * Destructor deallocates the member properties.
	 */
	~State();

	/**
	 * Erases the old list of lights and uses the new one.
	 *
	 * @param newLights the vector of new lights to use
	 */
	void setLights(std::vector<PointLight>* newLights);

	/**
	 * 
	 */
	void setShadeModel(const shade_model value);

	/**
	 * 
	 */
	shade_model getShadeModel() const;
	
	/**
	 * 
	 */
	void setLightModel(const light_model value);

	/**
	 * 
	 */
	light_model getLightModel() const;

	/**
	 * 
	 */
	void enableLighting(bool value = true);

	/**
	 * 
	 */
	bool getLighting() const { return this->m_lightingEnabled; }

	/**
	 * 
	 */
	void enableDepthTest(bool value = true);

	/**
	 * 
	 */
	bool getDepthTest() const { return this->m_depthTestEnabled; }

	/**
	 * 
	 */
	void enableTexturing2D(bool value = true);

	/**
	 * 
	 */
	bool getTexturing2D() const { return this->m_texture2dEnabled; }

	/**
	 * Accessor method for the global ambient intensity
	 */
	void setAmbientIntensity(float value);

	/**
	 * Accessor method for the global specular exponent
	 */
	void setSpecularExponent(float value);

	/**
	 * Accessor method for the global ambient intensity
	 */
	void setSpecularColor(cg::vecmath::Color3f* color);
	
	/**
	 * Accessor method for the list of lights.
	 * 
	 * @return reference for vector of lights.
	 */
	std::vector<PointLight>& getLights() const { return *lights; }

	/**
	 * Accessor method for the global ambient intensity
	 */
	float getAmbientIntensity() const { return this->ambientIntensity; }

	/**
	 * Accessor method for the global specular exponent
	 */
	float getSpecularExponent() const { return this->specularExponent; }

	/**
	 * Accessor method for the global ambient intensity
	 */
	cg::vecmath::Color3f& getSpecularColor() const { return *(this->specularColor); }
	
	/**
	 * Singleton interface
	 */
	static State* getInstance() {
		if(instance==NULL){
			instance = new State();
		}
		return instance;
	}
	
protected:
	float ambientIntensity;					//!< The global ambient lighting intensity.
	float specularExponent;					//!< The global specular component of the lighting model.
	cg::vecmath::Color3f* specularColor;	//!< The global specular color of the global environment light.
	std::vector<PointLight>* lights;		//!< The list of lights used for shading.
	
private:
	/**
	 * Constructor simply sets the two member properties from inputs.
	 * 
	 * @param pos the 3D position of the light
	 * @param intens the color of the light
	 */
	State();
	static State* instance;
	
	shade_model m_shadeModel;
	light_model m_lightModel;
	bool m_lightingEnabled;
	bool m_depthTestEnabled;
	bool m_texture2dEnabled;
	unsigned m_activeTextureUnit;
	
};

}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, pixelpipe::State& s)
{
	return out << "[ State ]";
}

#endif	// __PIPELINE_STATE_H