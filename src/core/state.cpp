#include <string>
#include <math.h>

#include "core/state.h"

namespace pixelpipe {
	
State* State::instance = NULL;
	
using namespace cg::vecmath;

State::State()
{	
	ambientIntensity = 0.1;
	specularColor = new Color3f(0.4, 0.4, 0.4);
	specularExponent = 40.0;
	
	this->lights = new std::vector<PointLight>();
	DEV() << "the lights: " << this->lights->size();
}

State::~State()
{
	// delete values ...
}

void State::setAmbientIntensity(float value)
{
	this->ambientIntensity = value;
}

void State::setSpecularExponent(float value)
{
	this->specularExponent = value;
}

void State::setSpecularColor(Color3f* color)
{
	this->specularColor = color;
}

void State::setLights(std::vector<PointLight>* newLights)
{
	if(lights!=NULL && newLights != this->lights) {
		delete lights;
		lights = NULL;
	}

	lights = newLights;
}

}	// namespace pixelpipe