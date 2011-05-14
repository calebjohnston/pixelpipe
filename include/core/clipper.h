#ifndef __PIPELINE_CLIPPER_H
#define __PIPELINE_CLIPPER_H

#include "core/vertex.h"

namespace pipeline {

class Clipper {
	
public:
	
	Clipper();
	
	/**
	 * Initializes a new clipper with a given number of attributes.
	 * 
	 * @param newNa The number of attributes.
	 */
	Clipper(int newNa);
	~Clipper();
	
	int clip(const Vertex* f, Vertex* fOut1, Vertex* fOut2);
	
protected:
	/** Number of user-supplied attributes */
	int na;
	
	/** Temporaries for clipping */
	Vertex fClip[3];
	
private:
	
};	// class Clipper
}	// namespace pipeline

#endif	// __PIPELINE_CLIPPER_H