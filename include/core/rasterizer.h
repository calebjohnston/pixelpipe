#ifndef __PIPELINE_RASTERIZER_H
#define __PIPELINE_RASTERIZER_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "fragment/frag_processor.h"
#include "cg/vecmath/vec4.hpp"

namespace pixelpipe {

/*!
 * \class Rasterizer "core/rasterizer.h"
 * \brief the class responsible for blitting geometry to a buffer
 *
 * This class is responsible for interpolating the attributes given to it across
 * the triangle, and handing off the correctly interpolated values to the
 * fragment processor. Clipping also happens within this class.
 * 
 * @see http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
 */
class Rasterizer {
public:
	/**
	 * The only constructor.
	 * 
	 * @param newNa The number of user defined attributes.
	 * @param newNx The width of the image.
	 * @param newNy The height of the image.
	 */
	Rasterizer(int newNa, int newNx, int newNy);
	~Rasterizer();
	
	/**
	 * The function used for blitting the geometry data to a framebuffer.
	 * 
	 * @param vs The array of vertices for rasterizing (only 3 at a time)
	 * @param fp A reference to the fragment processor to use for shading.
	 * @param fb A reference to the framebuffer to which the fragment will be written.
	 */
	void rasterize(const Vertex* vs, FragmentProcessor& fp, FrameBuffer& fb);

	/**
	 * Accessor method for the number of attributes that are given for each fragment.
	 * 
	 * @param count the new count
	 */
	void setAttributeCount(int count);
	
	
protected:
	int m_attributes;	//!< the number of attributes to be expected for each vertex being rasterized
	int m_frameWidth;	//!< the width of the target framebuffer
	int m_frameHeight;	//!< the height of the target framebuffer
	float* m_vData;		//!< The array of vertex & attribute floats that are computed during rasterization
	float* m_xInc;		//!< The x increment value used during the rasterization process.
	float* m_yInc;		//!< The y increment value used during the rasterization process.
	float* m_rowData;	//!< The local copy of row data used during the rasterization process.
	float* m_pixData;	//!< The local copy of fragment data used during the rasterization process.
	Fragment* m_frag;	//!< The fragment to be written to the framebuffer after rasterization.
	
private:
	static int ceil(float x);
	static int floor(float x);
	static float min(float a, float b, float c);
	static float max(float a, float b, float c);
	
};

}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::Rasterizer& r)
{
	return out << "[ Rasterizer ]";
}

#endif	// __PIPELINE_RASTERIZER_H