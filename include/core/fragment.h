#ifndef __PIPELINE_FRAGMENT_H
#define __PIPELINE_FRAGMENT_H

#include <stdlib.h>
#include <iostream>

namespace pixelpipe {

/*!
 * \class Fragment "core/fragment.h"
 * \brief A simple object to store a fragment using a specified number of attributes.
 * \see pixelpipe::Vertex
 */
struct Fragment {
public:	
	/**
	 * The constructor allocates the data required to store the per-vertex attribues.
	 */
	Fragment(int n=0) {
		length = n;
		y = x = -1;
		if(length > 0){
			attributes = (float*) malloc(length*sizeof(float));
		}
	}
	
	/**
	 * De-allocates the data used to store the fragment attribues.
	 */
	~Fragment(){
		free(attributes);
	}

	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Fragment ]";
	}
	
	int x;				//!< The screen space x coordinate of this fragment.
	int y;				//!< TThe screen space y coordinate of this fragment.
	float* attributes;	//!< TThe attributes associated with this fragment.
	int length;			//!< TThe number of attributes associated with this fragment
};

}

#endif	// __PIPELINE_FRAGMENT_H