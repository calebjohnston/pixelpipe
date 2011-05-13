#include "vertex/vert_frag_textured.h"

namespace pipeline {
	
TexturedFragmentShadedVP::TexturedFragmentShadedVP()
{
	size = 9 + 6*Pipeline.lights.size();
}

void TexturedFragmentShadedVP::vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
{
	//???
	super.vertex(v,new Color3f(0,0,0),n,t,output);
	output.setAttrs(size);
	output.attrs[0] = t.x;
	output.attrs[1] = t.y;
}

void TexturedFragmentShadedVP::triangle(Vector3f[] vs, Color3f[] cs_ign, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], NULL, ns[k], ts[k], outputs[k]);
	}
}

}