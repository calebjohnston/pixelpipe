#ifndef __PIPELINE_TEXTURED_PHONG_FRAG_H
#define __PIPELINE_TEXTURED_PHONG_FRAG_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec2.hpp"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/pointlight.h"
#include "fragment/frag_processor.h"

namespace pixelpipe {

/*!
 * \class TexturedPhongFP "fragment/frag_textured_phong.h"
 * \brief This FP does a texture lookup and a phone shading pass afterwards. 
 * 
 * The textured phong shader implements the z-buffer test, phone shading model, 
 * and a texture lookup in order to render the fragment. 
 */
class TexturedPhongFP : public FragmentProcessor
{
public:
	TexturedPhongFP();
	virtual int nAttr() const { return size; }
	virtual void fragment(Fragment& f, FrameBuffer& fb);
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ TexturedPhongFragmentProcessor ]";
	}
	
protected:
	int size;							//!< the size of the parameters that must be sent to the rasterizer.
	float nDotH;						//!< used for storing the dot product of the normal vector with the half vector
	float nDotL;                        //!< used for storing the dot product of the normal vector with the light vector
	float specularIntensity;			//!< the specularly intensity of the light using the phong model
	cg::vecmath::Color3f outColor;		//!< the local temporary used for computing the color
	cg::vecmath::Color3f texColor;		//!< the local temporary used for storing the results of the texture sample
	cg::vecmath::Vector3f normal;		//!< the local temporary for the normal at the fragment
	cg::vecmath::Vector3f viewVector;	//!< the local temporary for the view vector at the fragment
	cg::vecmath::Vector3f lightVector;	//!< the local temporary for the light vector at the fragment
	cg::vecmath::Vector3f halfVector;	//!< the local temporary for the half vector at the fragment
};

}

#endif	// __PIPELINE_TEXTURED_PHONG_FRAG_H