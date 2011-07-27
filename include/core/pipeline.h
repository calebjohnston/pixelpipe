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

/*!
 * \class Pipeline "core/pipeline.h"
 * \brief The pipeline class establishes the graphics rendering process from input 
 * geometry and textures to an output framebuffer.
 * 
 */
class Pipeline {
public:
	~Pipeline();
	
	/**
	 * Configures the pipeline so that the triangle and fragment processors are
	 * now up to date. Forces some reinitialization in order to set up things like
	 * the clipper and the rasterizer.
	 * 
	 * @param fpClass The class of the new fragment shader.
	 * @param vpClass The class of the new triangle shader.
	 */
	// void configure(Class fpClass, Class vpClass);
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	bool validConfiguration();
	
	/**
	 * @return a boolean flag representing the current shading model
	 */
	bool isFlatShaded();
	
//	Class getTriangleClass();
	
	/**
	 * Erases the old list of lights and uses the new one.
	 *
	 * @param newLights the vector of new lights to use
	 */
	void setLights(std::vector<PointLight>* newLights)
	{
		if(lights!=NULL) {
			delete lights;
			lights = NULL;
		}
		
		lights = newLights;
	}
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	void setTexture(const Texture& texture);
	
	/**
	 * Accessor method to change the current fragment processor.
	 *
	 * @param fragProc the new fragment processor to use
	 */
	void setFragmentProcessor(const FragmentProcessor* fragProc);
	
	/**
	 * Accessor method to change the current vertex processor.
	 *
	 * @param vertProc the new vertex processor to use
	 */
	void setVertexProcessor(const VertexProcessor* vertProc);
	
	/**
	 * Clears the current frame buffer.
	 */
	void clearFrameBuffer();
	
	/**
	 * Accessor method for the framebuffer.
	 *
	 * @return a constant pointer to the raw frame data.
	 */
	const char* getFrameData();
	
	/**
	 * Accessor method for the framebuffer instance.
	 *
	 * @return a reference to the framebuffer object
	 */
	FrameBuffer& getFrameBuffer() const { return *framebuffer; }
	
	/**
	 * Sets the modelview matrix to the identity, and notifies the vertex processor of 
	 * the change.
	 */
	void loadIdentity();
	
	/**
	 * Performs a rotation on the model view matrix using the supplied axis and 
	 * angle parameters
	 *
	 * @param angle the angle to rotate about the axis
	 * @param axis the axis about which to rotate
	 */
	void rotate(float angle, const cg::vecmath::Vector3f& axis);
	
	/**
	 * Performs a translation on the model view matrix using the supplied 3-vector.
	 *
	 * @param delta the vector representing the translation from the current position.
	 */
	void translate(const cg::vecmath::Vector3f& delta);
	
	/**
	 * Performs a scaling on the model view matrix using the supplied 3-vector.
	 *
	 * @param delta the vector representing the scaling.
	 */
	void scale(const cg::vecmath::Vector3f& scale);
	
	/**
	 * Sets the modelview matrix to be equal to the indicated viewing matrix, and
	 * notifies the TP of the change.
	 * 
	 * @param eye The location of the eye.
	 * @param target The target at which the eye is looking.
	 * @param up A vector that is not parallel to (target - eye) so as to indicate
	 *          which direction is up.
	 */
	void lookAt(cg::vecmath::Vector3f eye, cg::vecmath::Vector3f target, cg::vecmath::Vector3f up);
	
	
	/**
	 * Sets the projection matrix to represent the indicated viewing volume, and
	 * notifies the TP of the change.
	 * 
	 * @param l The left extent of the view volume.
	 * @param r The right extent of the view volume.
	 * @param b The bottom extent of the view volume.
	 * @param t The top extent of the view volume.
	 * @param n The near plane of the view volume.
	 * @param f The far plane of the view volume.
	 */
	void frustum(float l, float r, float b, float t, float n, float f);
	
	/**
	 * Sets the viewport matrix to the indicated window on screen, and notifies
	 * the TP of the change.
	 * 
	 * @param x The x location of the window.
	 * @param y The y location of the window.
	 * @param w The width of the window.
	 * @param h The height of the window.
	 */
	void viewport(int x, int y, int w, int h);
	
	
	/**
	 * Sets the pipeline mode to render a particular type of primitive.
	 */
	void begin(int primType);
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	void vertex(const cg::vecmath::Vector3f& v, const cg::vecmath::Color3f& c, const cg::vecmath::Vector3f& n, const cg::vecmath::Vector2f& t);
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	void end();
	
	
	/**
	 * Renders a triangle to the software pipeline.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	void renderTriangle(const cg::vecmath::Vector3f* v, const cg::vecmath::Color3f* c, const cg::vecmath::Vector3f* n, const cg::vecmath::Vector2f* t);
	
	std::vector<PointLight>& getLights() { return *lights; }
	
	static Pipeline* getInstance();
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Pipeline ]";
	}
	
	cg::vecmath::Matrix4f modelviewMatrix;	//!< The model-view matrix.
	cg::vecmath::Matrix4f projectionMatrix;	//!< The projection matrix.
	cg::vecmath::Matrix4f viewportMatrix;	//!< The viewport matrix.
	
	float ambientIntensity;	//!< The global ambient lighting intensity.
	float specularExponent;	//!< The global specular component of the lighting model.
	cg::vecmath::Color3f specularColor;	//!< The global specular color of the global environment light.

protected:
	int vertexIndex;	//!< The index of the vertex as determined by the drawing mode.
	int stripParity;	//!< The flag for triangle strip management
	int mode;			//!< The drawing mode (TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUAD, QUAD_STRIP)
	std::vector<PointLight>* lights;	//!< The list of lights used for shading.
	
	/**
	 * Notifies the TP of any changes to the modelview, projection, or viewing
	 * matrices.
	 */
	void recomputeMatrix();
	
	Pipeline(int nx=800, int ny=600, std::vector<PointLight>* lights=NULL);
	
private:
	// Class[] EMPTY_CLASS_ARRAY;
	// Object[] EMPTY_OBJECT_ARRAY;
	
	VertexProcessor* vp;		//!< The current vertex processor being used.
	Clipper* clipper;			//!< The geometry clipper being used to perform frustum culling.
	Rasterizer* rasterizer;		//!< An instance of the rasterizer being used to perform blitting.
	FragmentProcessor* fp;		//!< The current fragment processor being used.
	FrameBuffer* framebuffer;	//!< The current framebuffer being used as the render target.
	
	Vertex vertexCache[4];		//!< The vertex cache used to transfer geometry to through the pipeline.
	Vertex triangle1[3];		//!< The local copy of the first triangle stored after clipping.
	Vertex triangle2[3];		//!< The local copy of the second triangle stored after clipping.
	
	void swap(Vertex* va, int i, int j) const;
	
	/**
	 * Renders a triangle from already-processed vertices.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	void renderTriangle(const Vertex* vertices);
	
	static Pipeline* instance;	//!< The static singleton instance of the pipeline.
	
};	// class Pipeline

}	// namespace pipeline

#endif	// __PIPELINE_PIPELINE_H