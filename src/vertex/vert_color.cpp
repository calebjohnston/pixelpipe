#include "vertex/vert_color.h"

namespace pipeline {
	
void ConstColorVP::updateTransforms(const Pipeline& pipe)
{
	m.set(pipe.modelviewMatrix);
	m.leftCompose(pipe.projectionMatrix);
	m.leftCompose(pipe.viewportMatrix);
}

void ConstColorVP::triangle(cg::vecmath::Vector3f[] vs, cg::vecmath::Color3f[] cs, cg::vecmath::Vector3f[] ns_ign, cg::vecmath::Vector2f[] ts_ign, Vertex[] output)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], cs[k], NULL, NULL, output[k]);
	}
}

void ConstColorVP::vertex(cg::vecmath::Vector3f v, cg::vecmath::Color3f c, cg::vecmath::Vector3f n_ign, cg::vecmath::Vector2f t_ign, Vertex output)
{
	output.v.set(v.x, v.y, v.z, 1);
	m.rightMultiply(output.v);

	output.setAttrs(nAttr());
	output.attrs[0] = c.x;
	output.attrs[1] = c.y;
	output.attrs[2] = c.z;
}

}