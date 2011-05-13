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
	int nAttr() { return 3; }
	
	void fragment(const Fragment& f, FrameBuffer& fb);	
};

}

#endif	// #define __PIPELINE_COLOR_FRAG_H