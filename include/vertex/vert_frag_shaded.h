#ifndef __PIPELINE_FRAG_SHADED_PROCESSOR_H
#define __PIPELINE_FRAG_SHADED_PROCESSOR_H

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
 * Passes the normals and the colors of each vertex on to be rasterized, and
 * later shaded during the fragment stage of the pipeline. This results in the
 * highest quality images, but results in costly computation.
 * 
 * @author ags
 */
class FragmentShadedVP : public VertexProcessor {
public:	
	FragmentShadedVP();
	~FragmentShadedVP();
	virtual int nAttr() { return size; }
	virtual void updateTransforms(const Pipeline& pipe);
	virtual void triangle(	const cg::vecmath::Vector3f* vs, 
					const cg::vecmath::Color3f* cs, 
					const cg::vecmath::Vector3f* ns, 
					const cg::vecmath::Vector2f* ts, 
					Vertex* output);
	virtual void vertex(const cg::vecmath::Vector3f& v, 
				const cg::vecmath::Color3f& c, 
				const cg::vecmath::Vector3f& n, 
				const cg::vecmath::Vector2f& t, 
				Vertex& output);
	
protected:
	cg::vecmath::Matrix4f modelViewMatrix;
	cg::vecmath::Matrix4f m;
	cg::vecmath::Vector4f vert;
	cg::vecmath::Vector4f normal;
	cg::vecmath::Vector3f viewVector;
	cg::vecmath::Vector3f lightVector;
	cg::vecmath::Vector3f halfVector;
	cg::vecmath::Vector3f transformedNormal;
	cg::vecmath::Point3f transformedVertex;
	int size;
	
private:
	int position;
};

}

#endif	// __PIPELINE_FRAG_SHADED_PROCESSOR_H