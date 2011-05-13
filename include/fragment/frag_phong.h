#ifndef __PIPELINE_PHONG_FRAG_H
#define __PIPELINE_PHONG_FRAG_H

#include "cg/vecmath/color.h"
#include "cg/vecmath/vec3.h"
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
 * @author ags
 */
class PhongShadedFP : public FragmentProcessor {
public:
	PhongShadedFP();
	int nAttr() { return size; }
	void fragment(const Fragment& f, FrameBuffer& fb);
	
protected:	
	static int size;
	int position;
	float nDotL;
	float nDotH;
	float specularIntensity;
	Color3f outColor;
	Vector3f normal;
	Vector3f viewVector;
	Vector3f lightVector;
	Vector3f halfVector;
};

}

#endif	// #define __PIPELINE_PHONG_FRAG_H