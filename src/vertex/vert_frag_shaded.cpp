#include "vertex/vert_frag_shaded.h"

namespace pipeline {
	
FragmentShadedVP::FragmentShadedVP()
{
	size = 9 + 6*Pipeline.lights.size();
}


void FragmentShadedVP::updateTransforms(const Pipeline& pipe)
{
	// TODO
	modelViewMatrix = pipe.modelviewMatrix;		
	m.set(modelViewMatrix);
	m.leftCompose(pipe.projectionMatrix);
	m.leftCompose(pipe.viewportMatrix);
}

void FragmentShadedVP::vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
{
	// TODO
	output.setAttrs(nAttr());
	
	//output color
    output.attrs[0] = c.x;
    output.attrs[1] = c.y;
	output.attrs[2] = c.z;
	
	//transform vertex
	vertex.set(v.x,v.y,v.z,1.0f);
	modelViewMatrix.rightMultiply(vertex);
	transformedVertex.set(vertex.x,vertex.y,vertex.z);
	
	//transform normals
	n.normalize();
	normal.set(n.x,n.y,n.z,0.0f);
	modelViewMatrix.rightMultiply(normal);	  
	transformedNormal.set(normal.x,normal.y,normal.z);
	transformedNormal.normalize();
	
	//output the normal
	output.attrs[3] = transformedNormal.x;
	output.attrs[4] = transformedNormal.y;
	output.attrs[5] = transformedNormal.z;

	//calculate view vector
	viewVector.set(0.0f,0.0f,0.0f);
	viewVector.sub(viewVector,transformedVertex);
	viewVector.normalize();	
	
	//output the view vector
	output.attrs[6] = viewVector.x;
	output.attrs[7] = viewVector.y;
	output.attrs[8] = viewVector.z;

	//calculate light vectors
	int len = Pipeline.lights.size();
	for(int i=0; i<len; i++){
		position = 6*i;
		
		lightVector.set(Pipeline.lights.get(i).getPosition());
		lightVector.sub(lightVector,transformedVertex);
		lightVector.normalize();
		
		//output the light Vector
		output.attrs[9 + position] = lightVector.x;
		output.attrs[10 + position] = lightVector.y;
		output.attrs[11 + position] = lightVector.z;

		//calculate half vector
		halfVector.set(0, 0, 0);
		halfVector.set(viewVector);
		halfVector.add(lightVector);
		halfVector.normalize();
		
		//output the half vector
		output.attrs[12 + position] = halfVector.x;
		output.attrs[13 + position] = halfVector.y;
		output.attrs[14 + position] = halfVector.z;
	}
	
	output.v.set(v.x, v.y, v.z, 1.0f);
	m.rightMultiply(output.v);
}

void FragmentShadedVP::triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], cs[k], ns[k], NULL, outputs[k]);
	}
}

}