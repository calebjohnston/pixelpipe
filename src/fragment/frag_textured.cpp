#include "core/texture.h"
#include "fragment/frag_textured.h"

namespace pipeline {

void TexturedFP::fragment(const Fragment& f, FrameBuffer& fb)
{
	if(f.attrs[0] < fb.getZ(f.x, f.y)){
		color = texture.sample(new Vector2f(f.attrs[4], f.attrs[5]));
		color.x *= f.attrs[1];
		color.y *= f.attrs[2];
		color.z *= f.attrs[3];
		fb.set(f.x, f.y, color.x, color.y, color.z, f.attrs[0]);	  
	}
}

}