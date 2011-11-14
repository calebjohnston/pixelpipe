#include "vertex/vert_frag_shaded.h"

namespace pixelpipe {
	
using namespace cg::vecmath;
	
FragmentShadedVP::FragmentShadedVP() : VertexProcessor()
{
	size = 9 + 6 * Pipeline::getInstance()->getLights().size();
}

FragmentShadedVP::~FragmentShadedVP()
{
}

void FragmentShadedVP::updateTransforms(const Pipeline& pipe)
{
	// TODO
	modelViewMatrix = pipe.modelviewMatrix;		
	m = modelViewMatrix;
	Matrix4f temp = modelViewMatrix;
	m = pipe.projectionMatrix * temp;
	temp = m;
	m = pipe.viewportMatrix * temp;
}

void FragmentShadedVP::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t, Vertex& output)
{
	// TODO
	output.setAttrs(nAttr());
	
	//output color
    output.attributes[0] = c.x;
    output.attributes[1] = c.y;
	output.attributes[2] = c.z;
	
	//transform vertex
	vert.set(v.x,v.y,v.z,1.0f);
	Vector4f temp = vert;
	vert = modelViewMatrix * temp;
	//modelViewMatrix.rightMultiply(vert);
	
	transformedVertex.set(vert.x, vert.y, vert.z);
	
	//transform normals
	Vector3f temp3 = n;
	temp3.normalize();
	normal.set(temp3.x,temp3.y,temp3.z,0.0f);
	temp = normal;
	normal = modelViewMatrix * temp;
	transformedNormal.set(normal.x,normal.y,normal.z);
	transformedNormal.normalize();
	
	//output the normal
	output.attributes[3] = transformedNormal.x;
	output.attributes[4] = transformedNormal.y;
	output.attributes[5] = transformedNormal.z;

	//calculate view vector
	viewVector = 0.0;
	viewVector = viewVector - transformedVertex;
	viewVector.normalize();	
	
	//output the view vector
	output.attributes[6] = viewVector.x;
	output.attributes[7] = viewVector.y;
	output.attributes[8] = viewVector.z;

	//calculate light vectors
	int len = Pipeline::getInstance()->getLights().size();
	int position;
	for(int i=0; i<len; i++){
		position = 6*i;
		
		lightVector = Pipeline::getInstance()->getLights().at(i).getPosition();
		lightVector = lightVector - transformedVertex;
		lightVector.normalize();
		
		//output the light Vector
		output.attributes[9 + position] = lightVector.x;
		output.attributes[10 + position] = lightVector.y;
		output.attributes[11 + position] = lightVector.z;

		//calculate half vector
		halfVector = 0.0;
		halfVector = viewVector;
		halfVector += lightVector;
		halfVector.normalize();
		
		//output the half vector
		output.attributes[12 + position] = halfVector.x;
		output.attributes[13 + position] = halfVector.y;
		output.attributes[14 + position] = halfVector.z;
	}
	
	output.v.set(v.x, v.y, v.z, 1.0f);
	temp = output.v;
	output.v = m * temp;	// m.rightMultiply(output.v);
}

void FragmentShadedVP::triangle(const Vector3f* vs, const Color3f* cs, const Vector3f* ns, const Vector2f* ts, Vertex* output)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], cs[k], ns[k], Vector2f(), output[k]);
	}
}

}