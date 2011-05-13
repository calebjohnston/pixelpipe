#ifndef __PIPELINE_CONSTANT_COLOR_PROCESSOR_H
#define __PIPELINE_CONSTANT_COLOR_PROCESSOR_H

#include "cg/vecmath/vec2.h"
#include "cg/vecmath/vec3.h"
#include "cg/vecmath/mat4.h"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/vertex.h"
#include "vertex/vert_processor.h"

namespace pipeline {

/**
 * Sets each vertex up with a constant color, no matter what the viewing
 * conditions.
 * 
 * @author ags
 */
class ConstColorVP : public VertexProcessor {
public:	
	int nAttr() { return 3; }
	void updateTransforms(const Pipeline& pipe);
	void triangle(cg::vecmath::Vector3f[] vs, cg::vecmath::Color3f[] cs, cg::vecmath::Vector3f[] ns_ign, cg::vecmath::Vector2f[] ts_ign, Vertex[] output);
	void vertex(cg::vecmath::Vector3f v, cg::vecmath::Color3f c, cg::vecmath::Vector3f n_ign, cg::vecmath::Vector2f t_ign, Vertex output);
	
protected:
	/** This is the composed modelling, viewing, projection, and viewport matrix. */
	Matrix4f m;

};

}

#endif	// __PIPELINE_CONSTANT_COLOR_PROCESSOR_H
