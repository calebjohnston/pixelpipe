#ifndef __PIPELINE_VERTEX_H
#define __PIPELINE_VERTEX_H

#include <stdlib.h>
#include "cg/vecmath/vec4.hpp"

namespace pipeline {
	
/*!
 * \class Vertex "core/vertex.h"
 * \brief A simple object to store a vertex using a specified number of attributes.
 * \see pipeline::Fragment
 * 
 */
struct Vertex {
public:	
	/**
	 * The constructor allocates the data required to store the per-vertex attribues.
	 */
	Vertex(int n=0) {
		length = n;
		if(length > 0){
			attributes = (float*) malloc(length*sizeof(float));
		}
	}
	
	/**
	 * De-allocates the data used to store the fragment attribues.
	 */
	~Vertex(){
		free(attributes);
	}
	
	/**
	 * Sets the number of attributes for this vertex.
	 * 
	 * @param n The new number of attributes for this vertex.
	 */
	void setAttrs(int n)
	{
		if (attributes != NULL && n == length) return;
		length = n;
		attributes = (float*) malloc(length*sizeof(float));
	}
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Vertex: position=(" << v.x << "," << v.y << "," << v.z << "," << v.w << ") ]";
	}
	
	cg::vecmath::Vector4f v;	//!< The 4D homogenous position coordinate.
	float* attributes;			//!< The attributes associated with this vertex.
	int length;					//!< The total number of attributes associated with this vertex. 
};

}

#endif	// __PIPELINE_VERTEX_H