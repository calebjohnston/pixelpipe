#include "vertex/vert_textured_shaded.h"

namespace pipeline {

using namespace cg::vecmath;

void TexturedShadedVP::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t, Vertex& output)
{
	Color3f* color = new Color3f(0,0,0);
	SmoothShadedVP::vertex(v, *color, n, t, output);
	//output.setAttrs(nAttr());
	output.attributes[3] = t.x;
	output.attributes[4] = t.y;
}

}