#ifndef __PIPELINE_PIPELINE_H
#define __PIPELINE_PIPELINE_H

#include <vector>

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "core/pointlight.h"
#include "core/clipper.h"
#include "core/rasterizer.h"
#include "fragment/frag_processor.h"
#include "vertex/vert_processor.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"

namespace pipeline {

/**
 * This class directs the "pipeline process". Think of this class as describing
 * the hardware architecture.
 * 
 * @author ags
 */
class Pipeline {
public:
	~Pipeline();
	
	cg::vecmath::Matrix4f modelviewMatrix;
	cg::vecmath::Matrix4f projectionMatrix;
	cg::vecmath::Matrix4f viewportMatrix;
	
	float ambientIntensity;
	float specularExponent;
	cg::vecmath::Color3f specularColor;	// THERE IS NO Color3f type. See vecmath/color.h
	
	// void configure(Class fpClass, Class vpClass);
	bool validConfiguration();
	bool isFlatShaded();
//	Class getTriangleClass();
	void setTexture(const Texture& texture);
	void setFragmentProcessor(const FragmentProcessor* fragProc);
	void setVertexProcessor(const VertexProcessor* vertProc);
	void clearFrameBuffer();
	const char* getFrameData();
	FrameBuffer& getFrameBuffer() const { return *framebuffer; }
	void loadIdentity();
	
	void rotate(float angle, const cg::vecmath::Vector3f& axis);
	void translate(const cg::vecmath::Vector3f& delta);
	void scale(const cg::vecmath::Vector3f& scale);
	
	void lookAt(cg::vecmath::Vector3f eye, cg::vecmath::Vector3f target, cg::vecmath::Vector3f up);
	void frustum(float l, float r, float b, float t, float n, float f);
	void viewport(int x, int y, int w, int h);
	
	void begin(int primType);
	void vertex(const cg::vecmath::Vector3f& v, const cg::vecmath::Color3f& c, const cg::vecmath::Vector3f& n, const cg::vecmath::Vector2f& t);
	void end();
	void renderTriangle(const cg::vecmath::Vector3f* v, const cg::vecmath::Color3f* c, const cg::vecmath::Vector3f* n, const cg::vecmath::Vector2f* t);
	
	std::vector<PointLight>& getLights() { return *lights; }
	
	static Pipeline* getInstance();

protected:
	Pipeline(int nx=800, int ny=600, std::vector<PointLight>* lights=NULL);
	int vertexIndex;
	int stripParity;
	int mode;
	void recomputeMatrix();
	std::vector<PointLight>* lights;
	
private:
	// Class[] EMPTY_CLASS_ARRAY;
	// Object[] EMPTY_OBJECT_ARRAY;
	
	VertexProcessor* vp;
	
	Clipper* clipper;
	
	Rasterizer* rasterizer;
	
	FragmentProcessor* fp;
	
	FrameBuffer* framebuffer;
	
	Vertex vertexCache[4];
	Vertex triangle1[3];
	Vertex triangle2[3];
	
	void swap(Vertex* va, int i, int j) const;
	void renderTriangle(const Vertex* vertices);
	
	static Pipeline* instance;
	
};	// class Pipeline

}	// namespace pipeline

#endif	// __PIPELINE_PIPELINE_H