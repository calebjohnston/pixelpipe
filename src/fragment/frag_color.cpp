#include "fragment/frag_color.h"

namespace pipeline {
	
void ColorFP::fragment(Fragment& f, FrameBuffer& fb)
{
	fb.set(f.x, f.y, f.attributes[1], f.attributes[2], f.attributes[3], 0);
}

}