#include <math.h>
#include "vertex/vert_shaded.h"

namespace pixelpipe {

using namespace cg::vecmath;

SmoothShadedVP::SmoothShadedVP() : VertexProcessor()
{
	nDotH = 0;
	nDotL = 0;
}

void SmoothShadedVP::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t, Vertex& output)
{
	//transform vertex
	vert.set(v.x, v.y, v.z, 1.0f);
	Vector4f temp = vert;
	vert = modelViewMatrix * temp;
	transformedVertex.set(vert.x, vert.y, vert.z);

	//transform normals
	Vector3f temp3 = n;
	temp3.normalize();
	normal.set(temp3.x, temp3.y, temp3.z, 0.0f);
	temp = normal;
	normal = modelViewMatrix * temp;
	transformedNormal.set(normal.x, normal.y, normal.z);
	transformedNormal.normalize();

	//calculate view vector
	viewVector = 0.0;
	viewVector = viewVector - transformedVertex;
	viewVector.normalize();
	
	// we start with the ambient color.
	outColor.set(State::getInstance()->getAmbientIntensity(), State::getInstance()->getAmbientIntensity(), State::getInstance()->getAmbientIntensity());

	//calculate light vectors
	int len = State::getInstance()->getLights().size();
	for(int i=0; i<len; i++){
		lightVector = State::getInstance()->getLights().at(i).getPosition();
		lightVector = lightVector - transformedVertex;
		lightVector.normalize();

		//calculate N dot L
		nDotL = (float) dot(transformedNormal, lightVector);

		//add diffuse color for light 1
		outColor.x += c.x * nDotL * State::getInstance()->getLights().at(i).getIntensity().x;
		outColor.y += c.y * nDotL * State::getInstance()->getLights().at(i).getIntensity().y;
		outColor.z += c.z * nDotL * State::getInstance()->getLights().at(i).getIntensity().z;

		//calculate half vector
		halfVector = 0.0;
		halfVector = viewVector;
		halfVector += lightVector;
		halfVector.normalize();

		nDotH = (float) dot(transformedNormal, halfVector);

		//calculate specular intensity
		float specularIntensity = std::pow(nDotH, State::getInstance()->getSpecularExponent());
		if(specularIntensity < 0.0f){
			specularIntensity = 0.0f;
		}
		else if(specularIntensity > 1.0f){
			specularIntensity = 1.0f;
		}

		//add the specular color for light 1
		outColor.x += (State::getInstance()->getSpecularColor().x * specularIntensity);
		outColor.y += (State::getInstance()->getSpecularColor().y * specularIntensity);
		outColor.z += (State::getInstance()->getSpecularColor().z * specularIntensity);
	}
	
	//clamp colors
	if(outColor.x < 0.0f){
		outColor.x = State::getInstance()->getAmbientIntensity();
	}
	if(outColor.y < 0.0f){
		outColor.y = State::getInstance()->getAmbientIntensity();
	}
	if(outColor.z < 0.0f){
		outColor.z = State::getInstance()->getAmbientIntensity();
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
	output.v = MVP * temp;

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