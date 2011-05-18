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
	int nAttr() { return size; }
	void updateTransforms(const Pipeline& pipe);
	void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output);
	void triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs);
	
protected:
	Matrix4f modelViewMatrix;
	Matrix4f m;
	Vector4f vertex;
	Vector4f normal;
	Vector3f viewVector;
	Vector3f lightVector;
	Vector3f halfVector;
	Vector3f transformedNormal;
	Point3f transformedVertex;
	static int size;
	
private:
	int position;
};

}

#endif	// __PIPELINE_FRAG_SHADED_PROCESSOR_H