#ifndef __PIPELINE_SMOOTH_SHADED_PROCESSOR_H
#define __PIPELINE_SMOOTH_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/pointlight.h"
#include "core/vertex.h"
#include "vertex/vert_processor.h"

namespace pipeline {

/**
 * This triangle processor smoothly interpolates the color across the face of
 * the triangle. This is better than flat shading, but still not as nice as
 * fragment (aka phong) shading.
 * 
 * @author ags
 */
class SmoothShadedVP : public VertexProcessor {
public:
	SmoothShadedVP();
	virtual int nAttr() { return 3; }
	virtual void updateTransforms(const Pipeline& pipe);
	virtual void triangle(	const cg::vecmath::Vector3f* vs, 
					const cg::vecmath::Color3f* cs, 
					const cg::vecmath::Vector3f* ns_ign, 
					const cg::vecmath::Vector2f* ts_ign, 
					Vertex* output);
	virtual void vertex(const cg::vecmath::Vector3f& v, 
				const cg::vecmath::Color3f& c, 
				const cg::vecmath::Vector3f& n_ign, 
				const cg::vecmath::Vector2f& t_ign, 
				Vertex& output);
	
protected:	
	float nDotH;
	float nDotL;
	cg::vecmath::Matrix4f modelViewMatrix;
	cg::vecmath::Matrix4f m;
	cg::vecmath::Vector4f vert;
	cg::vecmath::Vector4f normal;
	cg::vecmath::Vector3f viewVector;
	cg::vecmath::Vector3f lightVector;
	cg::vecmath::Vector3f halfVector;
	cg::vecmath::Vector3f transformedNormal;
	cg::vecmath::Point3f transformedVertex;
	cg::vecmath::Color3f outColor;
	
};

}

#endif	// __PIPELINE_SMOOTH_SHADED_PROCESSOR_H