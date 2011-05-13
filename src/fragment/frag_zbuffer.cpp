#include "core/texture.h"
#include "fragment/frag_zbuffer.h"

namespace pipeline {

void ZBufferFP::fragment(const Fragment& f, FrameBuffer& fb)
{	
	if(f.attrs[0] < fb.getZ(f.x, f.y)){
		fb.set(f.x, f.y, f.attrs[1], f.attrs[2], f.attrs[3], f.attrs[0]);	  
	}
}

}