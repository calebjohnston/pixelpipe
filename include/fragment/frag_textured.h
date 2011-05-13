#ifndef __PIPELINE_TEXTURED_FRAG_H
#define __PIPELINE_TEXTURED_FRAG_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.h"
#include "cg/vecmath/vec2.h"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/pointlight.h"
#include "core/pipeline.h"
#include "fragment/frag_processor.h"

namespace pipeline {

/**
 * This FP does a texture lookup rather to determine the color of a fragment. It
 * also uses the z-buffering technique to draw the correct fragment.
 * 
 * @author ags
 */
class TexturedFP : public FragmentProcessor
{
public:
	int nAttr() { return 5; }
	void fragment(const Fragment& f, FrameBuffer& fb);
	
protected:
	Color3f color;
};

}

#endif	// __PIPELINE_TEXTURED_FRAG_H