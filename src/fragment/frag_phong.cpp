#include <math.h>
#include "fragment/frag_phong.h"

namespace pixelpipe {

PhongShadedFP::PhongShadedFP()
{
	size = 9 + 6 * State::getInstance()->getLights().size();
}

void PhongShadedFP::fragment(Fragment& f, FrameBuffer& fb)
{
	if(f.attributes[0] < fb.getZ(f.x, f.y)){
		//get normal
		normal.x = f.attributes[4];
		normal.y = f.attributes[5];
		normal.z = f.attributes[6];
		normal.normalize();
		
		//get viewVector
		viewVector.x = f.attributes[7];
		viewVector.y = f.attributes[8];
		viewVector.z = f.attributes[9];
		viewVector.normalize();
				
		//add lighting
		outColor.set(0.0,0.0,0.0);
		int position;
		for(int i = 0; i < State::getInstance()->getLights().size(); i++)
		{	
			position = 6*i;
			//get lightVector
			lightVector.x = f.attributes[10 + position];
			lightVector.y = f.attributes[11 + position];
			lightVector.z = f.attributes[12 + position];				
			lightVector.normalize();
			
			//get halfVector
			halfVector.x = f.attributes[13 + position];
			halfVector.y = f.attributes[14 + position];
			halfVector.z = f.attributes[15 + position];	
			halfVector.normalize();
			
			//compute dot products
			nDotL = dot(normal, lightVector);
			nDotH = dot(normal, halfVector);	
			
	   		//add diffuse color
	   		outColor.x += f.attributes[1] * nDotL * State::getInstance()->getLights().at(i).getIntensity().x;
	   		outColor.y += f.attributes[2] * nDotL * State::getInstance()->getLights().at(i).getIntensity().y;
	   		outColor.z += f.attributes[3] * nDotL * State::getInstance()->getLights().at(i).getIntensity().z;
	   
	   		//calculate specular intensity
			specularIntensity = std::pow(nDotH, State::getInstance()->getSpecularExponent());
			if(specularIntensity < 0.0){
				specularIntensity = 0.0;
			}
			else if(specularIntensity > 1.0){
				specularIntensity = 1.0;
			}
	   
			//add specular
			outColor.x += (State::getInstance()->getSpecularColor().x * specularIntensity);
			outColor.y += (State::getInstance()->getSpecularColor().y * specularIntensity);
			outColor.z += (State::getInstance()->getSpecularColor().z * specularIntensity);		   
		}	
	
		//clamp colors
		if(outColor.x < 0.0f){
			outColor.x = 0.0f;
		}
		if(outColor.y < 0.0f){
			outColor.y = 0.0f;
		}
		if(outColor.z < 0.0f){
			outColor.z = 0.0f;
		}
	
		//add ambient
		outColor.x += State::getInstance()->getAmbientIntensity();
		outColor.y += State::getInstance()->getAmbientIntensity();
		outColor.z += State::getInstance()->getAmbientIntensity();
	
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
	
		fb.set(f.x, f.y, outColor.x, outColor.y, outColor.z, f.attributes[0]);	
	}
}

}