#ifndef __PIPELINE_VERTEX_PROCESSOR_H
#define __PIPELINE_VERTEX_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/color.h"
#include "core/pipeline_software.h"
#include "core/vertex.h"
#include "core/state.h"

namespace pixelpipe {
	
	class SoftwarePipeline;
	
/*!
 * \class VertexProcessor "vertex/vert_processor.h"
 * \brief VertexProcessor is a base class for all vertex processors.
 * 
 * This class is a wrapper for the code that does triangle processing -- or
 * vertex processing, in the more traditional pipeline terminology. Its mission
 * is to transform a triangle into screen space vertex coordinates for the
 * rasterizer and to translate the vertex attributes supplied by the caller into
 * a set of attributes that will provide the necessary information for the
 * fragment processing stage.
 */
class VertexProcessor {
public:
	VertexProcessor();
	~VertexProcessor();
	
	/**
	 * Returns the number of attributes this triangle processor will provide.
	 * 
	 * @return The number of attributes this triangle processor will provide.
	 */
	virtual int nAttr() const = 0;
	
	/**
	 * We can access everything we need to know about the pipeline state: 
	 * the current transformation matrices and the lighting parameters, via 
	 * the Pipeline reference above. But for efficiency we may want to do some
	 * precomputation. This function will be called by the pipeline to notify
	 * this object whenever the transforms parameters are changed.
	 * 
	 * @param pipe The reference to the pipeline instance. Can be used to determine the necessary matrices.
	 */
	virtual void updateTransforms(const SoftwarePipeline& pipe);
	
	/**
	 * We can access everything we need to know about the pipeline state -- the
	 * current transformation matrices and the lighting parameters -- via the
	 * Pipeline reference above. But for efficiency we may want to do some
	 * precomputation. This function will be called by the pipeline to notify this
	 * object whenever the lighting parameters are changed.
	 * 
	 * @param pipe The reference to the pipeline instance. Can be used to determine the viewing conditions.
	 */
	virtual void updateLightModel(const SoftwarePipeline& pipe) { }
	
	/**
	 * This is the main function of this class, which is called once for every
	 * triangle in the scene. As input we get all the attributes of the triangle's
	 * three vertices, and as a result this function should call the rasterizer
	 * with appropriate vertex positions and attribute values.
	 * 
	 * @param v The three vertices in 3D object coordinates.
	 * @param c Colors associated with each of the vertices.
	 * @param n A normal for each vertex.
	 * @param t Texture coordinates for each vertex.
	 * @param output The array of 3 vertices that serves as the output for this method.
	 */
	virtual void triangle(const cg::vecmath::Vector3f* v, const cg::vecmath::Color3f* c, const cg::vecmath::Vector3f* n, const cg::vecmath::Vector2f* t, Vertex* output) = 0;
	
	/**
	 * This is the main function of this class, which is called once for every
	 * vertex. This routine takes the provided vertex data and prepares a 
	 * transformed vertex with attributes, ready to be sent to the rasterizer.
	 * 
	 * @param v The vertex position in 3D object coordinates.
	 * @param c The color associated with the vertex (null if unused).
	 * @param n The vertex normal (null if unused).
	 * @param t Texture coordinates for each vertex (null if unused).
	 * @param output The processed vertex.
	 */
	virtual void vertex(const cg::vecmath::Vector3f& v, const cg::vecmath::Color3f& c, const cg::vecmath::Vector3f& n, const cg::vecmath::Vector2f& t, Vertex& output) = 0;
	
protected:
	cg::vecmath::Matrix4f modelViewMatrix;	//!< the local model-view matrix
	cg::vecmath::Matrix4f MVP;				//!< the modelview * projection * viewport matrix
	
};

}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::VertexProcessor& vp)
{
	return out << "[ VertexProcessor ]";
}

#endif	// __PIPELINE_VERTEX_PROCESSOR_H