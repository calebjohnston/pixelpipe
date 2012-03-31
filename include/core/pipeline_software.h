#ifndef __PIPELINE_SOFTWARE_PIPELINE_H
#define __PIPELINE_SOFTWARE_PIPELINE_H

#include <vector>

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "core/pointlight.h"
#include "core/clipper.h"
#include "core/rasterizer.h"
// #include "core/pipeline_software.h"
#include "fragment/frag_processor.h"
#include "vertex/vert_processor.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"

namespace pixelpipe {
	
	class VertexProcessor;
	class FragmentProcessor;

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
	virtual void configure() = 0;

	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	virtual bool validConfiguration() = 0;

	/**
	 * @return a boolean flag representing the current shading model
	 */
	virtual bool isFlatShaded() = 0;

	/**
	 * Accessor method to change the current fragment processor.
	 *
	 * @param fragProc the new fragment processor to use
	 */
	virtual void setFragmentProcessor(const FragmentProcessor* fragProc) = 0;

	/**
	 * Accessor method to change the current vertex processor.
	 *
	 * @param vertProc the new vertex processor to use
	 */
	virtual void setVertexProcessor(const VertexProcessor* vertProc) = 0;

	/**
	 * Clears the current frame buffer.
	 */
	virtual void clearFrameBuffer() = 0;

	/**
	 * Accessor method for the framebuffer.
	 *
	 * @return a constant pointer to the raw frame data.
	 */
	virtual const char* getFrameData() = 0;

	/**
	 * Accessor method for the framebuffer instance.
	 *
	 * @return a reference to the framebuffer object
	 */
	virtual FrameBuffer& getFrameBuffer() const = 0;


	/**
	 * Performs a rotation on the model view matrix using the supplied axis and 
	 * angle parameters
	 *
	 * @param angle the angle to rotate about the axis
	 * @param axis the axis about which to rotate
	 */
	virtual void rotate(float angle, const cg::vecmath::Vector3f& axis) = 0;

	/**
	 * Performs a translation on the model view matrix using the supplied 3-vector.
	 *
	 * @param delta the vector representing the translation from the current position.
	 */
	virtual void translate(const cg::vecmath::Vector3f& delta) = 0;

	/**
	 * Performs a scaling on the model view matrix using the supplied 3-vector.
	 *
	 * @param delta the vector representing the scaling.
	 */
	virtual void scale(const cg::vecmath::Vector3f& scale) = 0;


	/**
	 * Sets the modelview matrix to be equal to the indicated viewing matrix, and
	 * notifies the TP of the change.
	 * 
	 * @param eye The location of the eye.
	 * @param target The target at which the eye is looking.
	 * @param up A vector that is not parallel to (target - eye) so as to indicate
	 *          which direction is up.
	 * @see http://www.opengl.org/wiki/GluLookAt_code
	 */
	virtual void lookAt(cg::vecmath::Vector3f eye, cg::vecmath::Vector3f target, cg::vecmath::Vector3f up) = 0;


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
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glFrustum.xml
	 */
	virtual void frustum(float l, float r, float b, float t, float n, float f) = 0;

	/**
	 * Sets the viewport matrix to the indicated window on screen, and notifies
	 * the TP of the change.
	 * 
	 * @param x The x location of the window.
	 * @param y The y location of the window.
	 * @param w The width of the window.
	 * @param h The height of the window.
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glViewport.xml
	 */
	virtual void viewport(int x, int y, int w, int h) = 0;


	/**
	 * Sets the pipeline mode to render a particular type of primitive.
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glBegin.xml
	 */
	virtual void begin(int primType) = 0;

	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	virtual void vertex(const cg::vecmath::Vector3f& v, const cg::vecmath::Color3f& c, const cg::vecmath::Vector3f& n, const cg::vecmath::Vector2f& t) = 0;

	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glBegin.xml
	 */
	virtual void end() = 0;

	/**
	 * Renders a triangle to the software pipeline.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	virtual void renderTriangle(const cg::vecmath::Vector3f* v, const cg::vecmath::Color3f* c, const cg::vecmath::Vector3f* n, const cg::vecmath::Vector2f* t) = 0;

	cg::vecmath::Matrix4f modelviewMatrix;	//!< The model-view matrix.
	cg::vecmath::Matrix4f projectionMatrix;	//!< The projection matrix.
	cg::vecmath::Matrix4f viewportMatrix;	//!< The viewport matrix.
	
	static Pipeline* getInstance(int mode = 0) { return instance; }

protected:
	Pipeline(int nx=800, int ny=600) {};

	static Pipeline* instance;	//!< The static singleton instance of the pipeline.

	/**
	 * Renders a triangle from already-processed vertices.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	virtual void renderTriangle(const Vertex* vertices) = 0;

private:

};	// class Pipeline

/*!
 * \class Pipeline "core/pipeline.h"
 * \brief The pipeline class establishes the graphics rendering process from input 
 * geometry and textures to an output framebuffer.
 * 
 */
class SoftwarePipeline : public Pipeline {
public:
	friend class Pipeline;
	~SoftwarePipeline();
	
	/**
	 * Configures the pipeline so that the triangle and fragment processors are
	 * now up to date. Forces some reinitialization in order to set up things like
	 * the clipper and the rasterizer.
	 * 
	 * @param fpClass The class of the new fragment shader.
	 * @param vpClass The class of the new triangle shader.
	 */
	// void configure(Class fpClass, Class vpClass);
	virtual void configure();
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	virtual bool validConfiguration();

	virtual bool isFlatShaded();
	
//	Class getTriangleClass();
	
	/**
	 * Erases the old list of lights and uses the new one.
	 *
	 * @param newLights the vector of new lights to use
	 */
	virtual void setLights(std::vector<PointLight>* newLights)
	{
		if(lights!=NULL) {
			delete lights;
			lights = NULL;
		}
		
		lights = newLights;
	}
	
	/**
	 * Sets the current texture
	 *
	 * @param a constant texture reference
	 */
	virtual void setTexture(const Texture& texture);
	
	/**
	 * Accessor method to change the current fragment processor.
	 *
	 * @param fragProc the new fragment processor to use
	 */
	virtual void setFragmentProcessor(const FragmentProcessor* fragProc);
	
	/**
	 * Accessor method to change the current vertex processor.
	 *
	 * @param vertProc the new vertex processor to use
	 */
	virtual void setVertexProcessor(const VertexProcessor* vertProc);
	
	/**
	 * Clears the current frame buffer.
	 */
	virtual void clearFrameBuffer();
	
	/**
	 * Accessor method for the framebuffer.
	 *
	 * @return a constant pointer to the raw frame data.
	 */
	virtual const char* getFrameData();
	
	/**
	 * Accessor method for the framebuffer instance.
	 *
	 * @return a reference to the framebuffer object
	 */
	virtual FrameBuffer& getFrameBuffer() const { return *framebuffer; }
	
	/**
	 * Sets the modelview matrix to the identity, and notifies the vertex processor of 
	 * the change.
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glLoadIdentity.xml
	 */
	virtual void loadIdentity();
	
	virtual void rotate(float angle, const cg::vecmath::Vector3f& axis);
	
	virtual void translate(const cg::vecmath::Vector3f& delta);
	
	virtual void scale(const cg::vecmath::Vector3f& scale);
	
	virtual void lookAt(cg::vecmath::Vector3f eye, cg::vecmath::Vector3f target, cg::vecmath::Vector3f up);
	
	virtual void frustum(float l, float r, float b, float t, float n, float f);
	
	virtual void viewport(int x, int y, int w, int h);
	
	virtual void begin(int primType);
	
	virtual void vertex(const cg::vecmath::Vector3f& v, const cg::vecmath::Color3f& c, const cg::vecmath::Vector3f& n, const cg::vecmath::Vector2f& t);
	
	virtual void end();
	
	
	/**
	 * Renders a triangle to the software pipeline.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	virtual void renderTriangle(const cg::vecmath::Vector3f* v, const cg::vecmath::Color3f* c, const cg::vecmath::Vector3f* n, const cg::vecmath::Vector2f* t);
	
	std::vector<PointLight>& getLights() { return *lights; }
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ SoftwarePipeline ]";
	}
	/*
	cg::vecmath::Matrix4f modelviewMatrix;	//!< The model-view matrix.
	cg::vecmath::Matrix4f projectionMatrix;	//!< The projection matrix.
	cg::vecmath::Matrix4f viewportMatrix;	//!< The viewport matrix.
	*/
	
	float ambientIntensity;	//!< The global ambient lighting intensity.
	float specularExponent;	//!< The global specular component of the lighting model.
	cg::vecmath::Color3f specularColor;	//!< The global specular color of the global environment light.
	
	/**
	 * Singleton interface.
	 * 
	 * @return singleton instance of Pipeline
	 */
	// static SoftwarePipeline* getInstance() {
	// 	if(SoftwarePipeline::instance==NULL){
	// 		SoftwarePipeline::instance = new pixelpipe::SoftwarePipeline();
	// 	}
	// 	return SoftwarePipeline::instance;
	// }

protected:
	int vertexIndex;	//!< The index of the vertex as determined by the drawing mode.
	int stripParity;	//!< The flag for triangle strip management
	int mode;			//!< The drawing mode (TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUAD, QUAD_STRIP)
	std::vector<PointLight>* lights;	//!< The list of lights used for shading.
	
	/**
	 * Notifies the TP of any changes to the modelview, projection, or viewing
	 * matrices.
	 */
	virtual void recomputeMatrix();
	
	/**
	 * Swaps two vertices within an array of vertices using index values
	 * 
	 * @param va an array of Vertex instances
	 * @param i the first index to use
	 * @param j the second index to use
	 */
	virtual void swap(Vertex* va, int i, int j) const
	{
		Vertex temp = va[i];
		va[i] = va[j];
		va[j] = temp;
	}
	
	SoftwarePipeline(int nx=800, int ny=600, std::vector<PointLight>* lights=NULL);
	
	virtual void renderTriangle(const Vertex* vertices);
	
private:
	// Class[] EMPTY_CLASS_ARRAY;
	// Object[] EMPTY_OBJECT_ARRAY;
	
	Clipper* clipper;			//!< The geometry clipper being used to perform frustum culling.
	Rasterizer* rasterizer;		//!< An instance of the rasterizer being used to perform blitting.
	VertexProcessor* vp;		//!< The current vertex processor being used.
	FragmentProcessor* fp;		//!< The current fragment processor being used.
	FrameBuffer* framebuffer;	//!< The current framebuffer being used as the render target.
	
	Vertex vertexCache[4];		//!< The vertex cache used to transfer geometry to through the pipeline.
	Vertex triangle1[3];		//!< The local copy of the first triangle stored after clipping.
	Vertex triangle2[3];		//!< The local copy of the second triangle stored after clipping.
	
	// static SoftwarePipeline* instance;
	
};	// class Pipeline


/**
 * Singleton interface.
 * 
 * @return singleton instance of Pipeline
 */
// Pipeline* Pipeline::getInstance(int mode) {
// 	if(instance==NULL){
// 		switch(mode){
// 			case 0: instance = new pixelpipe::SoftwarePipeline();
// 		}
// 	}
// 	return instance;
// }

}	// namespace pixelpipe

#endif	// __PIPELINE_SOFTWARE_PIPELINE_H