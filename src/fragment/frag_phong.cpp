#include <math.h>
#include "core/pipeline.h"
#include "fragment/frag_phong.h"

namespace pipeline {

PhongShadedFP::PhongShadedFP()
{
	size = 9 + 6*Pipeline.lights.size();
}


void PhongShadedFP::fragment(const Fragment& f, FrameBuffer& fb)
{
	if(f.attrs[0] < fb.getZ(f.x, f.y)){
		//get normal
		normal.x = f.attrs[4];
		normal.y = f.attrs[5];
		normal.z = f.attrs[6];
		normal.normalize();
		
		//get viewVector
		viewVector.x = f.attrs[7];
		viewVector.y = f.attrs[8];
		viewVector.z = f.attrs[9];
		viewVector.normalize();
				
		//add lighting
		outColor.set(0.0f,0.0f,0.0f);
		for(int i = 0; i < Pipeline.lights.size(); i++)
		{	
			position = 6*i;
			//get lightVector
			lightVector.x = f.attrs[10 + position];
			lightVector.y = f.attrs[11 + position];
			lightVector.z = f.attrs[12 + position];				
			lightVector.normalize();
			
			//get halfVector
			halfVector.x = f.attrs[13 + position];
			halfVector.y = f.attrs[14 + position];
			halfVector.z = f.attrs[15 + position];	
			halfVector.normalize();
			
			//computer dot products
			nDotL = normal.dot(lightVector);
			nDotH = normal.dot(halfVector);	
			
	   		//add diffuse color
	   		outColor.x += f.attrs[1] * nDotL * Pipeline.lights.get(i).getIntensity().x;
	   		outColor.y += f.attrs[2] * nDotL * Pipeline.lights.get(i).getIntensity().y;
	   		outColor.z += f.attrs[3] * nDotL * Pipeline.lights.get(i).getIntensity().z;
	   
	   		//calculate specular intensity
			specularIntensity = std::pow(nDotH, Pipeline.specularExponent);
			if(specularIntensity < 0.0f){
				specularIntensity = 0.0f;
			}
			else if(specularIntensity > 1.0f){
				specularIntensity = 1.0f;
			}
	   
			//add specular
			outColor.x += (Pipeline.specularColor.x * specularIntensity);
			outColor.y += (Pipeline.specularColor.y * specularIntensity);
			outColor.z += (Pipeline.specularColor.z * specularIntensity);		   
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
		outColor.x += Pipeline.ambientIntensity;
		outColor.y += Pipeline.ambientIntensity;
		outColor.z += Pipeline.ambientIntensity;
	
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
	
		fb.set(f.x, f.y, outColor.x, outColor.y, outColor.z, f.attrs[0]);	
	}
}

}