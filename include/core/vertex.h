#ifndef __PIPELINE_VERTEX_H
#define __PIPELINE_VERTEX_H

#include <stdlib.h>
#include "cg/vecmath/vec4.h"

namespace pipeline {

struct Vertex {
public:
	Vertex(int n) {
		length = n;
		attributes = (float*) malloc(length*sizeof(float));
	}
	
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
	
	/** The 4D homogenous coordinate location. */
	cg::vecmath::Vector4f v;

	/** The attributes associated with this vertex. */
	float* attributes;
	
	/** The total number of attributes associated with this vertex. */
	int length;
};

#endif	// __PIPELINE_VERTEX_H