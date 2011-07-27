#ifndef __PIPELINE_POINTLIGHT_H
#define __PIPELINE_POINTLIGHT_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"

namespace pipeline {

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
	PointLight(cg::vecmath::Point3f pos, cg::vecmath::Color3f intens){
		position = new cg::vecmath::Point3f(pos);
		intensity = new cg::vecmath::Color3f(intens);
	}
	
	/**
	 * Destructor deallocates the member properties.
	 */
	~PointLight(){
		delete position;
		delete intensity;
	}

	/**
	 * Accessor method for the light position.
	 * 
	 * @return a reference to the light's position vector
	 */
	cg::vecmath::Point3f& getPosition() const { return *position; }

	/**
	 * Accessor method for the light color.
	 * 
	 * @return a reference to the light's color member
	 */
	cg::vecmath::Color3f& getIntensity() const { return *intensity; } 
	
	/**
	 * Accessor method for the light position.
	 * 
	 * @param pos a new position vector
	 */
	void setPosition(cg::vecmath::Point3f pos) { *intensity = pos; }
	
	/**
	 * Accessor method for the light color.
	 * 
	 * @param val a new color value
	 */
	void setIntensity(cg::vecmath::Color3f val) { *intensity = val; }
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ PointLight: position=(" << position->x << "," << position->y << "," << position->z << "), intensity=(" << intensity->x << "," << intensity->y << "," << intensity->z << ") ]";
	}
	
protected:
	cg::vecmath::Point3f* position;		//!< The position of the light.
	cg::vecmath::Color3f* intensity;	//!< The color intesity of the light source.
	
};

}

#endif	// __PIPELINE_POINTLIGHT_H