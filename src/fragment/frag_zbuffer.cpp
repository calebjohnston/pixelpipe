#include "core/texture.h"
#include "fragment/frag_zbuffer.h"

namespace pixelpipe {

void ZBufferFP::fragment(Fragment& f, FrameBuffer& fb)
{	
	if(f.attributes[0] < fb.getZ(f.x, f.y)){
		fb.set(f.x, f.y, f.attributes[1], f.attributes[2], f.attributes[3], f.attributes[0]);	  
	}
}

}