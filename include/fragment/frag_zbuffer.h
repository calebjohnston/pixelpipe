#ifndef __PIPELINE_Z_BUFFERED_FRAG_H
#define __PIPELINE_Z_BUFFERED_FRAG_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "fragment/frag_processor.h"

namespace pixelpipe {

/*!
 * \class ZBufferFP "fragment/frag_zbuffer.h"
 * \brief performs Z-buffer test before blitting fragment to framebuffer.
 * 
 * This fragment processor will place the indicated color into the framebuffer
 * only if the fragment passes the z buffer test, meaning if it isn't occluded 
 * by another fragment.
 * 
 */
class ZBufferFP : public FragmentProcessor {
public:
	virtual int nAttr() const { return 3; }
	virtual void fragment(Fragment& f, FrameBuffer& fb);
	
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
