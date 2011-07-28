#include "vertex/vert_frag_textured.h"

namespace pipeline {

using namespace cg::vecmath;
	
TexturedFragmentShadedVP::TexturedFragmentShadedVP()
{
	size = 9 + 6 * Pipeline::getInstance()->getLights().size();
}

void TexturedFragmentShadedVP::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t, Vertex& output)
{
	// this whole thing doens't work. 
	// We could get it to work much better if we were to separate out 
	// more of the functionality in the FragmentShadedVP::vertex function.
	// For instance, we need to only call Vertex::setAttrs once! Not twice!
	Color3f* color = new Color3f(0,0,0);
	FragmentShadedVP::vertex(v, *color, n, t, output);
	output.setAttrs(size);
	output.attributes[0] = t.x;
	output.attributes[1] = t.y;
}

void TexturedFragmentShadedVP::triangle(const Vector3f* vs, const Color3f* cs, const Vector3f* ns, const Vector2f* ts, Vertex* output)
{
	Color3f* color = new Color3f();
	
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], *color, ns[k], ts[k], output[k]);
	}
}

} 