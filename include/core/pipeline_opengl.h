#ifndef __PIPELINE_OPENGL_H
#define __PIPELINE_OPENGL_H

#include <vector>

#include "core/common.h"
#include "core/vertex.h"
#include "core/state.h"
#include "core/pipeline.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"

namespace pixelpipe {

/*!
 * \class OpenGLPipeline "core/pipeline_opengl.h"
 * \brief The pipeline class establishes the OpenGL hardware pipeline.
 * 
 */
class OpenGLPipeline : public Pipeline {
public:	
	OpenGLPipeline(int nx=800, int ny=600);
	~OpenGLPipeline();
	
	/**
	 * Configures the pipeline so that the triangle and fragment processors are
	 * now up to date. Forces some reinitialization in order to set up things like
	 * the clipper and the rasterizer.
	 */
	virtual void configure();

	/**
	 * Initializes OpenGL state variables for displaying the framebuffer 
	 * to the window.
	 */
	virtual void init();
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	virtual bool validConfiguration();
	
	/**
	 * @return a boolean flag representing the current shading model
	 */
	virtual bool isFlatShaded();
	
	/**
	 * Sets the current texture
	 *
	 * @param a constant texture reference
	 */
	virtual void setTexture(const Texture& texture);
	
	/**
	 * Clears the current frame buffer.
	 */
	virtual void clearFrameBuffer();

	/**
	 * Draws the current frame buffer. Or with double buffering, swaps the frame buffers.
	 */
	virtual void drawFrameBuffer();
	
	/**
	 * Accessor method for the framebuffer.
	 *
	 * @return a constant pointer to the raw frame data.
	 */
	virtual const char* getFrameData();
	
	/**
	 * ! @copydoc Pipeline::loadIdentity()
	 */
	virtual void loadIdentity();
	
	/**
	 * ! @copydoc Pipeline::rotate()
	 */
	virtual void rotate(float angle, const cg::vecmath::Vector3f& axis);
	
	/**
	 * ! @copydoc Pipeline::translate()
	 */
	virtual void translate(const cg::vecmath::Vector3f& delta);
	
	/**
	 * ! @copydoc Pipeline::scale()
	 */
	virtual void scale(const cg::vecmath::Vector3f& scale);
	
	/**
	 * ! @copydoc Pipeline::lookAt()
	 */
	virtual void lookAt(cg::vecmath::Vector3f eye, cg::vecmath::Vector3f target, cg::vecmath::Vector3f up);
	
	
	/**
	 * ! @copydoc Pipeline::frustum()
	 */
	virtual void frustum(float l, float r, float b, float t, float n, float f);

	/**
	 * ! @copydoc Pipeline::ortho()
	 */
	virtual void ortho(float l, float r, float b, float t, float n, float f);
	
	/**
	 * ! @copydoc Pipeline::viewport()
	 */
	virtual void viewport(int x, int y, int w, int h);
	
	/**
	 * ! @copydoc Pipeline::pushMatrix()
	 */
	virtual void pushMatrix(const cg::vecmath::Matrix4f* matrix = NULL);
	
	/**
	 * ! @copydoc Pipeline::popMatrix()
	 */
	virtual void popMatrix();
	
	/**
	 * ! @copydoc Pipeline::loadMatrix()
	 */
	virtual void loadMatrix(const cg::vecmath::Matrix4f& matrix);
	
	/**
	 * ! @copydoc Pipeline::loadTransposeMatrix()
	 */
	virtual void loadTransposeMatrix(const cg::vecmath::Matrix4f& matrix);
	
	/**
	 * ! @copydoc Pipeline::setMatrixMode()
	 */
	virtual void setMatrixMode(const matrix_mode mode);
	
	/**
	 * ! @copydoc Pipeline::multiplyMatrix()
	 */
	virtual void multiplyMatrix(const cg::vecmath::Matrix4f& matrix);
	
	/**
	 * ! @copydoc Pipeline::loadTransposeMatrixMultiply()
	 */
	virtual void loadTransposeMatrixMultiply(const cg::vecmath::Matrix4f& matrix);	
	
	/**
	 * Sets the pipeline mode to render a particular type of primitive.
	 */
	virtual void begin(const drawing_mode mode);
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	virtual void vertex(const cg::vecmath::Vector3f& v, const cg::vecmath::Color3f& c, const cg::vecmath::Vector3f& n, const cg::vecmath::Vector2f& t);
	
	/**
	 * Compares the vertex and fragment processors to makes sure the information passed
	 * between them is in the same in size. 
	 *
	 * @return a boolean flag representing the validity of the configuration.
	 */
	virtual void end();
	
	/**
	 * ! @copydoc Pipeline::clear()
	 */
	virtual void clear(const buffer_bit bit = BUFFER_COLOR);
	
	/**
	 * Renders a triangle to the software pipeline.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	virtual void renderTriangle(const cg::vecmath::Vector3f* v, const cg::vecmath::Color3f* c, const cg::vecmath::Vector3f* n, const cg::vecmath::Vector2f* t);
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ OpenGLPipeline ]";
	}

protected:
	
private:	
	// void swap(Vertex* va, int i, int j) const;
	
	/**
	 * Renders a triangle from already-processed vertices.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	// virtual void renderTriangle(const Vertex* vertices);
	
};	// class OpenGLPipeline

}	// namespace pixelpipe {

#endif	// __PIPELINE_OPENGL_H