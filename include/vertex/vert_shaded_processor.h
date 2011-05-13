#ifndef __PIPELINE_SMOOTH_SHADED_PROCESSOR_H
#define __PIPELINE_SMOOTH_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.h"
#include "cg/vecmath/vec3.h"
#include "cg/vecmath/vec4.h"
#include "cg/vecmath/mat4.h"
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
	int nAttr() { return 3; }
	void updateTransforms(const Pipeline& pipe);
	void vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output);
	void triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs);
	
protected:	
	float nDotH;
	float nDotL;
	Matrix4f modelViewMatrix;
	Matrix4f m;
	Vector4f vertex;
	Vector4f normal;
	Vector3f viewVector;
	Vector3f lightVector;
	Vector3f halfVector;
	Vector3f transformedNormal;
	Point3f transformedVertex;
	Color3f outColor;
	
};

}

#endif	// __PIPELINE_SMOOTH_SHADED_PROCESSOR_H