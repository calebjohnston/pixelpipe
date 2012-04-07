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
	
	m_shadeModel = SHADE_SMOOTH;
	m_lightModel = LIGHT_AMBIENT;
	m_lightingEnabled = false;
	m_depthTestEnabled = false;
	m_texture2dEnabled = false;
	
	this->lights = new std::vector<PointLight>();
}

State::~State()
{
	// delete values ...
}

void State::setShadeModel(const shade_model value)
{
	this->m_shadeModel = value;
}

shade_model State::getShadeModel() const
{
	return this->m_shadeModel;
}

void State::setLightModel(const light_model value)
{
	this->m_lightModel = value;
}

light_model State::getLightModel() const
{
	return this->m_lightModel;
}

void State::enableLighting(bool value)
{
	this->m_lightingEnabled = value;
}

void State::enableDepthTest(bool value)
{
	this->m_depthTestEnabled = value;
}

void State::enableTexturing2D(bool value)
{
	this->m_texture2dEnabled = value;
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
	if(this->lights!=NULL && newLights != this->lights) {
		delete this->lights;
		this->lights = NULL;
	}

	this->lights = newLights;
}

}	// namespace pixelpipe