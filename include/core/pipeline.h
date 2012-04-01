#ifndef __PIPELINE_PIPELINE_H
#define __PIPELINE_PIPELINE_H

#include <vector>

#include "core/common.h"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "core/pointlight.h"
#include "core/state.h"
#include "core/texture.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"

namespace pixelpipe {

/*!
 * \class Pipeline "core/pipeline.h"
 * \brief The pipeline class establishes the graphics rendering process from input 
 * geometry and textures to an output framebuffer.
 * 
 */
class Pipeline {
public:	
	// Pipeline(int nx=800, int ny=600);
	// ~Pipeline();
	
	/**
	 * Configures the pipeline so that the triangle and fragment processors are
	 * now up to date. Forces some reinitialization in order to set up things like
	 * the clipper and the rasterizer.
	 * 
	 * @param fpClass The class of the new fragment shader.
	 * @param vpClass The class of the new triangle shader.
	 */
	virtual void configure() = 0;

	/**
	 * Initializes OpenGL state variables for displaying the framebuffer 
	 * to the window.
	 */
	virtual void init() = 0;
	
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
	 * Sets the current texture
	 *
	 * @param a constant texture reference
	 */
	virtual void setTexture(const Texture& texture) = 0;
	
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
	 * Replaces the currently selected matrix with the identity matrix
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glLoadIdentity.xml
	 */
	virtual void loadIdentity() = 0;
	
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
	 * 
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
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glViewport.xml
	 */
	virtual void viewport(int x, int y, int w, int h) = 0;
	
	/**
	 * Multiply the currently selected matrix with an orthographic matrix
	 * 
	 * @param l The left extent of the view volume.
	 * @param r The right extent of the view volume.
	 * @param b The bottom extent of the view volume.
	 * @param t The top extent of the view volume.
	 * @param n The near plane of the view volume.
	 * @param f The far plane of the view volume.
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
	 */
	virtual void ortho(float l, float r, float b, float t, float n, float f) = 0;
	
	/**
	 * Add the input matrix to the the top of the matrix stack
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glPushMatrix.xml
	 */
	virtual void pushMatrix(const cg::vecmath::Matrix4f* matrix = NULL) = 0;
	
	/**
	 * Removes the matrix at the top of the matrix stack
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glPushMatrix.xml
	 */
	virtual void popMatrix() = 0;
	
	/**
	 * Replaces the currently selected matrix with the specified matrix
	 * 
	 * @param matrix a constant reference to a 4x4 matrix to load
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glLoadMatrix.xml
	 */
	virtual void loadMatrix(const cg::vecmath::Matrix4f& matrix) = 0;
	
	/**
	 * Replaces the currently matrix with the specified row-major ordered matrix
	 * 
	 * @param matrix a constant reference to a 4x4 matrix to load
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glLoadTransposeMatrix.xml
	 */
	virtual void loadTransposeMatrix(const cg::vecmath::Matrix4f& matrix) = 0;
	
	/**
	 * Specify which matrix is the currently selected matrix
	 * 
	 * @param mode a constant representing the matrix to select
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glMatrixMode.xml
	 */
	virtual void setMatrixMode(const matrix_mode mode) = 0;
	
	/**
	 * Multiply the currently selected matrix with the specified matrix
	 * 
	 * @param matrix a constant reference to a 4x4 matrix to multiply
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glMultMatrix.xml
	 */
	virtual void multiplyMatrix(const cg::vecmath::Matrix4f& matrix) = 0;
	
	/**
	 * Multiply the currently selected matrix with the specified row-major 
	 * ordered matrix
	 * 
	 * @param matrix a constant reference to a 4x4 matrix to multiply
	 * 
	 * @see http://www.opengl.org/sdk/docs/man/xhtml/glMultTransposeMatrix.xml
	 */
	virtual void loadTransposeMatrixMultiply(const cg::vecmath::Matrix4f& matrix) = 0;
	
	
	/**
	 * Sets the pipeline mode to render a particular type of primitive.
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
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Pipeline ]";
	}

protected:
	
private:
	/*
	Vertex vertexCache[4];		//!< The vertex cache used to transfer geometry to through the pipeline.
	Vertex triangle1[3];		//!< The local copy of the first triangle stored after clipping.
	Vertex triangle2[3];		//!< The local copy of the second triangle stored after clipping.
	*/
	
	/**
	 * Renders a triangle from already-processed vertices.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	virtual void renderTriangle(const Vertex* vertices) = 0;
	
};	// class Pipeline

}	// namespace pixelpipe {

#endif	// __PIPELINE_PIPELINE_H