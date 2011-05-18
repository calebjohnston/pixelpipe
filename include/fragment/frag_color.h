#ifndef __PIPELINE_COLOR_FRAG_H
#define __PIPELINE_COLOR_FRAG_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "fragment/frag_processor.h"

namespace pipeline {

/**
 * This fragment program will render the fragments color into the framebuffer
 * regardless of whether it is in front of an earlier fragment.
 * 
 * @author ags
 */
class ColorFP : public FragmentProcessor {
public:	
	virtual int nAttr() const { return 3; }
	
	virtual void fragment(Fragment& f, FrameBuffer& fb);	
};

}	// namespace pipeline

#endif	// #define __PIPELINE_COLOR_FRAG_H