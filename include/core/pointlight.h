#ifndef __PIPELINE_POINTLIGHT_H
#define __PIPELINE_POINTLIGHT_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"

namespace pipeline {

class PointLight {
public:
	PointLight(cg::vecmath::Point3f pos, cg::vecmath::Color3f intens){
		position = new cg::vecmath::Point3f(pos);
		intensity = new cg::vecmath::Color3f(intens);
	}
	~PointLight(){
		delete position;
		delete intensity;
	}

	cg::vecmath::Point3f& getPosition() const { return *position; }
	cg::vecmath::Color3f& getIntensity() const { return *intensity; } 
	void setPosition(cg::vecmath::Point3f pos) { *intensity = pos; }
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