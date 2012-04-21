#ifndef __PIPELINE_SOFTWARE_H
#define __PIPELINE_SOFTWARE_H

#include <list>

#include "core/common.h"
#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "core/clipper.h"
#include "core/state.h"
#include "core/pipeline.h"
#include "core/rasterizer.h"
#include "fragment/frag_processor.h"
#include "vertex/vert_processor.h"
#include "cg/vecmath/color.h"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"

namespace pixelpipe {

class VertexProcessor;
class FragmentProcessor;

/*!
 * \class SoftwarePipeline "core/pipeline_software.h"
 * \brief The SoftwarePipeline class establishes the graphics rendering process from input 
 * geometry and textures to an output framebuffer all performed on the CPU.
 * 
 */
class SoftwarePipeline : public Pipeline {
public:	
	SoftwarePipeline(int nx=800, int ny=600);
	~SoftwarePipeline();
	
	/**
	 * Configures the pipeline so that the triangle and fragment processors are
	 * now up to date. Forces some reinitialization in order to set up things like
	 * the clipper and the rasterizer.
	 * 
	 */
	virtual void configure();

	/**
	 * Initializes OpenGL state variables for binding the framebuffer 
	 * to the window using GLUT.
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
	 * Draws the current frame buffer.
	 */
	virtual void drawFrameBuffer();
	
	/**
	 * Accessor method for the framebuffer.
	 *
	 * @return a constant pointer to the raw frame data.
	 */
	virtual const void* getFrameData();
	
	/**
	 * Accessor method for the framebuffer instance.
	 *
	 * @return a reference to the framebuffer object
	 */
	virtual FrameBuffer& getFrameBuffer() const { return *m_framebuffer; }
	
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
	virtual void pushMatrix(cg::vecmath::Matrix4f* matrix = NULL);
	
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
	 * ! @copydoc Pipeline::setActiveTexture()
	 */
	virtual void setActiveTexture(unsigned unit);

	/**
	 * ! @copydoc Pipeline::getActiveTexture()
	 */
	virtual unsigned getActiveTexture() const;

	/**
	 * ! @copydoc Pipeline::generateTexture()
	 */
	virtual unsigned generateTexture();

	/**
	 * ! @copydoc Pipeline::deleteTexture()
	 */
	virtual void deleteTexture(unsigned* texture);

	/**
	 * ! @copydoc Pipeline::bindTexture()
	 */
	virtual void bindTexture(unsigned texture);

	/**
	 * ! @copydoc Pipeline::loadTexture2D()
	 */
	virtual void loadTexture2D(const unsigned width, const unsigned height, const pixel_format format, const pixel_type type, const void* data);

	/**
	 * ! @copydoc Pipeline::getViewportMatrix()
	 */
	virtual cg::vecmath::Matrix4f& viewportMatrix() const { return (*this->m_viewportMatrix); }

	/**
	 * ! @copydoc Pipeline::getModelViewMatrix()
	 */
	virtual cg::vecmath::Matrix4f& modelViewMatrix() const { return (*this->m_modelviewMatrix); }

	/**
	 * ! @copydoc Pipeline::getProjectionMatrix()
	 */
	virtual cg::vecmath::Matrix4f& projectionMatrix() const { return (*this->m_projectionMatrix); }

	/**
	 * ! @copydoc Pipeline::getViewportMatrix()
	 */
	virtual cg::vecmath::Matrix4f getViewportMatrix();

	/**
	 * ! @copydoc Pipeline::getModelViewMatrix()
	 */
	virtual cg::vecmath::Matrix4f getModelViewMatrix();

	/**
	 * ! @copydoc Pipeline::getProjectionMatrix()
	 */
	virtual cg::vecmath::Matrix4f getProjectionMatrix();
	
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

protected:
	matrix_mode m_matrixMode;		//!< The currently selected matrix mode.
	int m_vertexIndex;				//!< The index of the vertex as determined by the drawing mode.
	int m_stripParity;				//!< The flag for triangle strip management
	drawing_mode m_mode;			//!< The drawing mode (TRIANGLES, TRIANGLE_STRIP, TRIANGLE_FAN, QUAD, QUAD_STRIP)
	cg::vecmath::Matrix4f* m_modelviewMatrix;			//!< The model-view matrix.
	cg::vecmath::Matrix4f* m_projectionMatrix;			//!< The projection matrix.
	cg::vecmath::Matrix4f* m_viewportMatrix;			//!< The viewport matrix.
	cg::vecmath::Matrix4f* m_currentMatrix;				//!< The currently selected matrix using the MatrixMode methods.
	std::list<cg::vecmath::Matrix4f*>* m_modelviewStack;	//!< A LIFO stack of user-defined modelview matrices
	std::list<cg::vecmath::Matrix4f*>* m_projectionStack;	//!< A LIFO stack of user-defined projection matrices
	std::vector<Texture*>* m_textureUnits;	//!< The set of texture units that can be used for texture mapping
	int m_textureIndex;	//!< The currently selected texture unit index
	
	/**
	 * Notifies the TP of any changes to the modelview, projection, or viewing
	 * matrices.
	 */
	void recomputeMatrix();
	
private:
	VertexProcessor* m_vp;			//!< The current vertex processor being used.
	Clipper* m_clipper;				//!< The geometry clipper being used to perform frustum culling.
	Rasterizer* m_rasterizer;		//!< An instance of the rasterizer being used to perform blitting.
	FragmentProcessor* m_fp;		//!< The current fragment processor being used.
	FrameBuffer* m_framebuffer;		//!< The current framebuffer being used as the render target.
	
	Vertex m_vertexCache[4];		//!< The vertex cache used to transfer geometry to through the pipeline.
	Vertex m_triangle1[3];			//!< The local copy of the first triangle stored after clipping.
	Vertex m_triangle2[3];			//!< The local copy of the second triangle stored after clipping.
	
	void swap(Vertex* va, int i, int j) const;
	
	/**
	 * Renders a triangle from already-processed vertices.
	 * 
	 * @param v The 3 vertices of the triangle.
	 * @param c The 3 colors of the triangle - one for each vertex.
	 * @param n The 3 normals of the triangle - one for each vertex.
	 * @param t The 3 texture coordinates of the triangle - one for each vertex.
	 */
	virtual void renderTriangle(const Vertex* vertices);
	
};	// class Pipeline

}	// namespace pixelpipe

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::SoftwarePipeline& pipe)
{
	return out << "[ SoftwarePipeline ]";
}

#endif	// __PIPELINE_SOFTWARE_H