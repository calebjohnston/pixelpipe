#include <math.h>
#include "vertex/vert_shaded_processor.h"

namespace pipeline {
	
SmoothShadedVP::SmoothShadedVP()
{
	nDotH = 0;
	nDotL = 0;
}

void SmoothShadedVP::updateTransforms(const Pipeline& pipe)
{
	modelViewMatrix = pipe.modelviewMatrix;		
	m.set(modelViewMatrix);
	m.leftCompose(pipe.projectionMatrix);
	m.leftCompose(pipe.viewportMatrix);
	
}

void SmoothShadedVP::vertex(Vector3f v, Color3f c, Vector3f n, Vector2f t, Vertex output)
{
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

	//calculate view vector
	viewVector.set(0.0f,0.0f,0.0f);
	viewVector.sub(viewVector,transformedVertex);
	viewVector.normalize();
	
	// we start with the ambient color.
	outColor.set(Pipeline.ambientIntensity, Pipeline.ambientIntensity, Pipeline.ambientIntensity);

	//calculate light vectors
	int len = Pipeline.lights.size();
	for(int i=0; i<len; i++){
		lightVector.set(Pipeline.lights.get(i).getPosition());
		lightVector.sub(lightVector,transformedVertex);
		lightVector.normalize();

		//calculate N dot L
		nDotL = (float) transformedNormal.dot(lightVector);

		//add diffuse color for light 1
		outColor.x += c.x * nDotL * Pipeline.lights.get(i).getIntensity().x;
		outColor.y += c.y * nDotL * Pipeline.lights.get(i).getIntensity().y;
		outColor.z += c.z * nDotL * Pipeline.lights.get(i).getIntensity().z;

		//calculate half vector
		halfVector.set(0, 0, 0);
		halfVector.set(viewVector);
		halfVector.add(lightVector);
		halfVector.normalize();

		nDotH = (float) transformedNormal.dot(halfVector);

		//calculate specular intensity
		float specularIntensity = std::pow(nDotH, Pipeline.specularExponent);
		if(specularIntensity < 0.0f){
			specularIntensity = 0.0f;
		}
		else if(specularIntensity > 1.0f){
			specularIntensity = 1.0f;
		}

		//add the specular color for light 1
		outColor.x += (Pipeline.specularColor.x * specularIntensity);
		outColor.y += (Pipeline.specularColor.y * specularIntensity);
		outColor.z += (Pipeline.specularColor.z * specularIntensity);
	}
	
	//clamp colors
	if(outColor.x < 0.0f){
		outColor.x = Pipeline.ambientIntensity;
	}
	if(outColor.y < 0.0f){
		outColor.y = Pipeline.ambientIntensity;
	}
	if(outColor.z < 0.0f){
		outColor.z = Pipeline.ambientIntensity;
	}

	//clamp colors
	if(outColor.x > 1.0f){
		outColor.x = 1.0f;
	}
	if(outColor.y > 1.0f){
		outColor.y = 1.0f;
	}
	if(outColor.z > 1.0f){
		outColor.z = 1.0f;
	}
	
	//output the calculations
	output.setAttrs(nAttr());
	output.v.set(v.x, v.y, v.z, 1.0f);
	m.rightMultiply(output.v);	

	output.attrs[0] = outColor.x;
	output.attrs[1] = outColor.y;
	output.attrs[2] = outColor.z; 
}

void SmoothShadedVP::triangle(Vector3f[] vs, Color3f[] cs, Vector3f[] ns, Vector2f[] ts, Vertex[] outputs)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], cs[k], ns[k], ts[k], outputs[k]);
	}
}

}