#ifndef __PIPELINE_POINTLIGHT_H
#define __PIPELINE_POINTLIGHT_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"

namespace pixelpipe {

/*!
 * \class PointLight "core/pointlight.h"
 * \brief A simple class to contain lighting variables (such as position and intensity)
 * 
 */
class PointLight {
public:
	/**
	 * Constructor simply sets the two member properties from inputs.
	 * 
	 * @param pos the 3D position of the light
	 * @param intens the color of the light
	 */
	PointLight(cg::vecmath::Point3f pos, cg::vecmath::Color3f intens)
	{
		m_position = new cg::vecmath::Point3f(pos);
		m_intensity = new cg::vecmath::Color3f(intens);
	}
	
	/**
	 * Copy constructor
	 */
	PointLight(const PointLight& light)
	{
		*this = light;
	}

	/**
	 * Copy constructor
	 */
	PointLight& operator=(const PointLight& light)
	{
		m_position = new cg::vecmath::Point3f(light.getPosition());
		m_intensity = new cg::vecmath::Color3f(light.getIntensity());
		
		return *this;
	}
	
	/**
	 * Destructor deallocates the member properties.
	 */
	~PointLight(){
		delete m_position;
		delete m_intensity;
	}

	/**
	 * Accessor method for the light position.
	 * 
	 * @return a reference to the light's position vector
	 */
	cg::vecmath::Point3f& getPosition() const { return *m_position; }

	/**
	 * Accessor method for the light color.
	 * 
	 * @return a reference to the light's color member
	 */
	cg::vecmath::Color3f& getIntensity() const { return *m_intensity; } 
	
	/**
	 * Accessor method for the light position.
	 * 
	 * @param pos a new position vector
	 */
	void setPosition(cg::vecmath::Point3f pos) { *m_intensity = pos; }
	
	/**
	 * Accessor method for the light color.
	 * 
	 * @param val a new color value
	 */
	void setIntensity(cg::vecmath::Color3f val) { *m_intensity = val; }
	
protected:
	cg::vecmath::Point3f* m_position;	//!< The position of the light.
	cg::vecmath::Color3f* m_intensity;	//!< The color intesity of the light source.
	
};

}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::PointLight& p)
{
	return out << "[ PointLight: position=" << p.getPosition() << ", intensity=" << p.getIntensity() << " ]";
}

#endif	// __PIPELINE_POINTLIGHT_H