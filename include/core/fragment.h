#ifndef __PIPELINE_FRAGMENT_H
#define __PIPELINE_FRAGMENT_H

#include <stdlib.h>

namespace pipeline {

struct Fragment {
public:
	Fragment(int n) {
		length = n;
		attributes = (float*) malloc(length*sizeof(float));
	}
	
	~Fragment(){
		free(attributes);
	}
	
	/** The screen space x coordinate of this fragment. */
	int x;

	/** The screen space y coordinate of this fragment. */
	int y;

	/** The attributes associated with this fragment. */
	float* attributes;

	/** The number of attributes associated with this fragment */
	int length;

};

}

#endif	// __PIPELINE_FRAGMENT_H