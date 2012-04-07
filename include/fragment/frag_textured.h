#ifndef __PIPELINE_TEXTURED_FRAG_H
#define __PIPELINE_TEXTURED_FRAG_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec2.hpp"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/pointlight.h"
#include "fragment/frag_processor.h"

namespace pixelpipe {

/*!
 * \class TexturedFP "fragment/frag_textured.h"
 * \brief This FP does a texture lookup rather to determine the color of a fragment. It
 * also uses the z-buffering technique to draw the correct fragment.
 * 
 */
class TexturedFP : public FragmentProcessor
{
public:
	virtual int nAttr() const { return 5; }
	virtual void fragment(Fragment& f, FrameBuffer& fb);
	
protected:
	cg::vecmath::Color3f color;	//!< local temporary color value sampled from the texture
};

}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::TexturedFP& fp)
{
	return out << "[ TexturedFragmentProcessor ]";
}

#endif	// __PIPELINE_TEXTURED_FRAG_H