#ifndef __PIPELINE_CONSTANT_COLOR_PROCESSOR_H
#define __PIPELINE_CONSTANT_COLOR_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/mat4.hpp"
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
	int nAttr() const { return 3; }
	void updateTransforms(const Pipeline& pipe);
	void triangle(	const cg::vecmath::Vector3f* vs, 
					const cg::vecmath::Color3f* cs, 
					const cg::vecmath::Vector3f* ns_ign, 
					const cg::vecmath::Vector2f* ts_ign, 
					Vertex* output);
	void vertex(const cg::vecmath::Vector3f& v, 
				const cg::vecmath::Color3f& c, 
				const cg::vecmath::Vector3f& n_ign, 
				const cg::vecmath::Vector2f& t_ign, 
				Vertex& output);
	
protected:
	/** This is the composed modelling, viewing, projection, and viewport matrix. */
	cg::vecmath::Matrix4f mvp;

};

}

#endif	// __PIPELINE_CONSTANT_COLOR_PROCESSOR_H
