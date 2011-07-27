#ifndef __PIPELINE_TEXTURED_FRAG_H
#define __PIPELINE_TEXTURED_FRAG_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec2.hpp"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/pointlight.h"
#include "core/pipeline.h"
#include "fragment/frag_processor.h"

namespace pipeline {

/*!
 * \class TexturedFP "fragment/frag_textured.h"
 * \brief This FP does a texture lookup rather to determine the color of a fragment. It
 * also uses the z-buffering technique to draw the correct fragment.
 * 
 */
class TexturedFP : public FragmentProcessor
{
public:
	int nAttr() { return 5; }
	void fragment(Fragment& f, FrameBuffer& fb);
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ TexturedFragmentProcessor ]";
	}
	
protected:
	cg::vecmath::Color3f color;	//!< local temporary color value sampled from the texture
};

}

#endif	// __PIPELINE_TEXTURED_FRAG_H