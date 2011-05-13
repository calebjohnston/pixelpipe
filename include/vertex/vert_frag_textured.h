#ifndef __PIPELINE_TEXTURED_FRAG_SHADED_PROCESSOR_H
#define __PIPELINE_TEXTURED_FRAG_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.h"
#include "cg/vecmath/vec3.h"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/vertex.h"
#include "vertex/vert_frag_shaded.h"

namespace pipeline {

/**
 * This is the same triangle processor as FragmentShadedTP, but it also
 * interpolates texture coordinates, allowing for texturing to be done in the
 * fragment stage.
 * 
 * @author ags
 */
class TexturedFragmentShadedVP : public FragmentShadedVP {	
public:
	TexturedFragmentShadedVP();
	int nAttr() { return size; }
	void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output);
	void triangle(Vector3f[] vs, Color3f[] cs_ign, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs);
	
private:
	static int size;

};

}

#endif	// __PIPELINE_TEXTURED_FRAG_SHADED_PROCESSOR_H
