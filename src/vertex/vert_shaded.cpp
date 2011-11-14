#include <math.h>
#include "vertex/vert_shaded.h"

namespace pixelpipe {

using namespace cg::vecmath;

SmoothShadedVP::SmoothShadedVP() : VertexProcessor()
{
	nDotH = 0;
	nDotL = 0;
}

void SmoothShadedVP::updateTransforms(const Pipeline& pipe)
{
	modelViewMatrix = pipe.modelviewMatrix;		
	m = modelViewMatrix;
	Matrix4f temp = modelViewMatrix;
	m = pipe.projectionMatrix * temp;
	temp = m;
	m = pipe.viewportMatrix * temp;
}

void SmoothShadedVP::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t, Vertex& output)
{
	//transform vertex
	vert.set(v.x,v.y,v.z,1.0f);
	Vector4f temp = vert;
	vert = modelViewMatrix * temp;
	transformedVertex.set(vert.x,vert.y,vert.z);

	//transform normals
	Vector3f temp3 = n;
	temp3.normalize();
	normal.set(temp3.x,temp3.y,temp3.z,0.0f);
	temp = normal;
	normal = modelViewMatrix * temp;
	transformedNormal.set(normal.x,normal.y,normal.z);
	transformedNormal.normalize();

	//calculate view vector
	viewVector = 0.0;
	viewVector = viewVector - transformedVertex;
	viewVector.normalize();
	
	// we start with the ambient color.
	outColor.set(Pipeline::getInstance()->ambientIntensity, Pipeline::getInstance()->ambientIntensity, Pipeline::getInstance()->ambientIntensity);

	//calculate light vectors
	int len = Pipeline::getInstance()->getLights().size();
	for(int i=0; i<len; i++){
		lightVector = Pipeline::getInstance()->getLights().at(i).getPosition();
		lightVector = lightVector - transformedVertex;
		lightVector.normalize();

		//calculate N dot L
		nDotL = (float) dot(transformedNormal, lightVector);

		//add diffuse color for light 1
		outColor.x += c.x * nDotL * Pipeline::getInstance()->getLights().at(i).getIntensity().x;
		outColor.y += c.y * nDotL * Pipeline::getInstance()->getLights().at(i).getIntensity().y;
		outColor.z += c.z * nDotL * Pipeline::getInstance()->getLights().at(i).getIntensity().z;

		//calculate half vector
		halfVector = 0.0;
		halfVector = viewVector;
		halfVector += lightVector;
		halfVector.normalize();

		nDotH = (float) dot(transformedNormal, halfVector);

		//calculate specular intensity
		float specularIntensity = std::pow(nDotH, Pipeline::getInstance()->specularExponent);
		if(specularIntensity < 0.0f){
			specularIntensity = 0.0f;
		}
		else if(specularIntensity > 1.0f){
			specularIntensity = 1.0f;
		}

		//add the specular color for light 1
		outColor.x += (Pipeline::getInstance()->specularColor.x * specularIntensity);
		outColor.y += (Pipeline::getInstance()->specularColor.y * specularIntensity);
		outColor.z += (Pipeline::getInstance()->specularColor.z * specularIntensity);
	}
	
	//clamp colors
	if(outColor.x < 0.0f){
		outColor.x = Pipeline::getInstance()->ambientIntensity;
	}
	if(outColor.y < 0.0f){
		outColor.y = Pipeline::getInstance()->ambientIntensity;
	}
	if(outColor.z < 0.0f){
		outColor.z = Pipeline::getInstance()->ambientIntensity;
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
	
	temp = output.v;
	output.v = m * temp;

	output.attributes[0] = outColor.x;
	output.attributes[1] = outColor.y;
	output.attributes[2] = outColor.z; 
}

void SmoothShadedVP::triangle(const Vector3f* vs, const Color3f* cs, const Vector3f* ns, const Vector2f* ts, Vertex* output)
{
	for (int k = 0; k < 3; k++) {
		vertex(vs[k], cs[k], ns[k], ts[k], output[k]);
	}
}

}