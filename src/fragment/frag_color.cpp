#include "fragment/frag_color.h"

namespace pipeline {
	
void ColorFP::fragment(const Fragment& f, FrameBuffer& fb)
{
	fb.set(f.x, f.y, f.attrs[1], f.attrs[2], f.attrs[3], 0);
}

}