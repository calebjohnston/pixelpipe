#ifndef __PIPELINE_POINTLIGHT_H
#define __PIPELINE_POINTLIGHT_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.h"

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
	
protected:
	cg::vecmath::Point3f* position;
	cg::vecmath::Color3f* intensity;
	
};

}

#endif	// __PIPELINE_POINTLIGHT_H