#include "core/texture.h"
#include "fragment/frag_textured.h"

namespace pixelpipe {

void TexturedFP::fragment(Fragment& f, FrameBuffer& fb)
{
	if(f.attributes[0] < fb.getZ(f.x, f.y)){
		color = m_texture->sample(f.attributes[4], f.attributes[5]);
		color.x *= f.attributes[1];
		color.y *= f.attributes[2];
		color.z *= f.attributes[3];
		fb.set(f.x, f.y, color.x, color.y, color.z, f.attributes[0]);	  
	}
}

}