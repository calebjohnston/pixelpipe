#ifndef __PIPELINE_CLIPPER_H
#define __PIPELINE_CLIPPER_H

#include "core/vertex.h"

namespace pixelpipe {

/*!
 * \class Clipper "core/clipper.h"
 * \brief The Clipper class is responsible for performing view volume clipping.
 * 
 */
class Clipper {
public:
	/**
	 * Default constructor.
	 */
	Clipper();
	
	/**
	 * Initializes a new clipper with a given number of attributes.
	 * 
	 * @param newNa The number of attributes.
	 */
	Clipper(int newNa);
	
	/**
	 * Destructor.
	 */
	~Clipper();
	
	/**
	 * The interface for the clipper. Each triangle will be clipped against the
	 * near plane, resulting in either 0, 1, or 2 triangles. The number of
	 * triangles will be returned, and the resulting vertices will be stored into
	 * fOut1 if only one triangle results, or in both fOut1 and fOut2 if two triangles
	 * result.
	 * 
	 * @param f The vertices of the triangle to be clipped.
	 * @param fOut1 The vertices of the first resulting triangle, if any.
	 * @param fOut2 The vertices of the second resulting triangle, if any.
	 * @return The number of resulting triangles.
	 */
	int clip(const Vertex* f, Vertex* fOut1, Vertex* fOut2);
	
	/**
	 * Accessor method for the number of attributes that are passed to each triangle 
	 * vertex after the clipping stage.
	 * 
	 * @param count the new count
	 */
	void setAttributeCount(unsigned count)
	{
		m_attributes = count;
	}
	
protected:
	unsigned m_attributes;	//!< Number of user-supplied attributes
	Vertex m_fClip[3];		//!< Temporaries for clipping
	
private:
	
};	// class Clipper

}	// namespace pixelpipe

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::Clipper& c)
{
	return out << "[ Clipper ]";
}

#endif	// __PIPELINE_CLIPPER_H