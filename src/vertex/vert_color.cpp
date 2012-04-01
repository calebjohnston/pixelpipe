#include "vertex/vert_color.h"

using namespace cg::vecmath;

namespace pixelpipe {
	
void ConstColorVP::updateTransforms(const SoftwarePipeline& pipe)
{
	mvp = pipe.modelviewMatrix;
	Matrix4f temp = mvp;
	mvp = pipe.projectionMatrix * temp;
	temp = mvp;
	mvp = pipe.viewportMatrix * temp;
}

void ConstColorVP::triangle(const Vector3f* vs, const Color3f* cs, const Vector3f* ns_ign, const Vector2f* ts_ign, Vertex* output)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], cs[k], ns_ign[0], ts_ign[0], output[k]);
	}
}

void ConstColorVP::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n_ign, const Vector2f& t_ign, Vertex& output)
{
	output.v.set(v.x, v.y, v.z, 1);
	cg::vecmath::Vector4f temp = output.v;
	output.v = mvp * temp;
	// mvp.rightMultiply(output.v);

	output.setAttrs(nAttr());
	output.attributes[0] = c.x;
	output.attributes[1] = c.y;
	output.attributes[2] = c.z;
}

}