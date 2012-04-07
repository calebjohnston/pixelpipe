#ifndef __PIPELINE_COLOR_FRAG_H
#define __PIPELINE_COLOR_FRAG_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "fragment/frag_processor.h"

namespace pixelpipe {

/*!
 * \class ColorFP "fragment/frag_color.h"
 * \brief Provides simple per-pixel color shading.
 * This fragment program will render the fragments color into the framebuffer
 * regardless of whether it is in front of an earlier fragment (ie. No Z buffer test)
 * 
 */
class ColorFP : public FragmentProcessor {
public:	
	virtual int nAttr() const { return 3; }
	
	virtual void fragment(Fragment& f, FrameBuffer& fb);
	
};

}	// namespace pixelpipe

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::ColorFP& fp)
{
	return out << "[ ColorFragmentProcessor ]";
}

#endif	// #define __PIPELINE_COLOR_FRAG_H