#ifndef __PIPELINE_PIPELINE_H
#define __PIPELINE_PIPELINE_H

#include <vector>

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "core/pointlight.h"
#include "fragment/frag_processor.h"
#include "vertex/vert_processor.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec4.h"
#include "cg/vecmath/mat4.h"

namespace pipeline {

/**
 * This class directs the "pipeline process". Think of this class as describing
 * the hardware architecture.
 * 
 * @author ags
 */
class Pipeline {
public:
	Pipeline(int nx, int ny, std::vector<PointLight> lights);
	~Pipeline();
	
	cg::vecmath::Matrix4f modelviewMatrix;
	cg::vecmath::Matrix4f projectionMatrix;
	cg::vecmath::Matrix4f viewportMatrix;
	
	std::vector<PointLight> lights;
	const float ambientIntensity;
	float specularExponent;
	cg::vecmath::Color3f specularColor;	// THERE IS NO Color3f type. See vecmath/color.h
	
	void configure(Class fpClass, Class vpClass);
	bool validConfiguration();
	bool isFlatShaded();
//	Class getTriangleClass();
	void setTexture(Texture texture);
	void clearFrameBuffer()
	byte[] getFrameData();
	void loadIdentity();
	
	void rotate(float angle, cg::vecmath::Vector3f axis);
	void translate(cg::vecmath::Vector3f v);
	void scale(cg::vecmath::Vector3f v);
	
	void lookAt(cg::vecmath::Vector3f eye, cg::vecmath::Vector3f target, cg::vecmath::Vector3f up);
	void frustum(float l, float r, float b, float t, float n, float f);
	void viewport(int x, int y, int w, int h);
	
	void begin(int primType);
	void vertex(cg::vecmath::Vector3f v, cg::vecmath::Color3f c, cg::vecmath::Vector3f n, cg::vecmath::Vector2f t);
	void end();
	void renderTriangle(cg::vecmath::Vector3f[] v, cg::vecmath::Color3f[] c, cg::vecmath::Vector3f[] n, cg::vecmath::Vector2f[] t);

protected:
	int vertexIndex;
	int stripParity;
	int mode;
	void recomputeMatrix();
	
private:
	Class[] EMPTY_CLASS_ARRAY;
	Object[] EMPTY_OBJECT_ARRAY;
	
	VertexProcessor* vp;
	
	Clipper* clipper;
	
	Rasterizer* rasterizer;
	
	FragmentProcessor* fp;
	
	FrameBuffer* framebuffer;
	
	Vertex[4] vertexCache;
	Vertex[3] triangle1;
	Vertex[3] triangle2;
	
	void swap(Vertex[] va, int i, int j);
	void renderTriangle(Vertex[] vertices);
	
};	// class Pipeline

}	// namespace pipeline

#endif	// __PIPELINE_PIPELINE_H