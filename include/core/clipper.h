#ifndef __PIPELINE_CLIPPER_H
#define __PIPELINE_CLIPPER_H

#include "core/vertex.h"

namespace pipeline {

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
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Clipper ]";
	}
	
protected:
	int na;				//!< Number of user-supplied attributes
	Vertex fClip[3];	//!< Temporaries for clipping
	
private:
	
};	// class Clipper
}	// namespace pipeline

#endif	// __PIPELINE_CLIPPER_H