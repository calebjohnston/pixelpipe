#ifndef __PIPELINE_Z_BUFFERED_FRAG_H
#define __PIPELINE_Z_BUFFERED_FRAG_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "fragment/frag_processor.h"

namespace pipeline {

/**
 * This fragment processor will place the indicated color into the framebuffer
 * only if the fragment passes the z buffer test (ie - it isn't occluded by
 * another fragment).
 * 
 */
class ZBufferFP : public FragmentProcessor {
public:
	int nAttr() { return 3; }
	void fragment(Fragment& f, FrameBuffer& fb);
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ ZBufferedFragmentProcessor ]";
	}
};

}

#endif	// __PIPELINE_Z_BUFFERED_FRAG_H
