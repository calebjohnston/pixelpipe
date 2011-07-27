#ifndef __PIPELINE_PHONG_FRAG_H
#define __PIPELINE_PHONG_FRAG_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/pointlight.h"
#include "core/pipeline.h"
#include "fragment/frag_processor.h"

namespace pipeline {

/**
 * This is the fragment program which actually uses a shading model to compute
 * the color on a per fragment basis.
 * 
 */
class PhongShadedFP : public FragmentProcessor {
public:
	PhongShadedFP();
	virtual int nAttr() const { return size; }
	virtual void fragment(Fragment& f, FrameBuffer& fb);
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ PhongShadedFragmentProcessor ]";
	}
	
protected:	
	int size;							//!< the size of the parameters that must be sent to the rasterizer.
	float nDotH;						//!< used for storing the dot product of the normal vector with the half vector
	float nDotL;                        //!< used for storing the dot product of the normal vector with the light vector
	float specularIntensity;			//!< the specularly intensity of the light using the phong model
	cg::vecmath::Color3f outColor;		//!< the local temporary used for computing the color
	cg::vecmath::Vector3f normal;		//!< the local temporary for the normal at the fragment
	cg::vecmath::Vector3f viewVector;	//!< the local temporary for the view vector at the fragment
	cg::vecmath::Vector3f lightVector;	//!< the local temporary for the light vector at the fragment
	cg::vecmath::Vector3f halfVector;	//!< the local temporary for the half vector at the fragment
};

}

#endif	// #define __PIPELINE_PHONG_FRAG_H