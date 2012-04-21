#ifndef __PIPELINE_SCENE_H
#define __PIPELINE_SCENE_H

#include <iostream>
#include <string>
#include <vector>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "core/pipeline.h"
#include "core/geometry.h"
#include "core/texture.h"
#include "core/common.h"

namespace pixelpipe {

/*!
 * \class Scene "core/scene.h"
 * 
 * A Scene object represents a collection of geometry and texture data. A Scene
 * object should know how to render itself into both an OpenGL context and a 
 * PixelPipe context.
 */
class Scene {
public:
	Scene(Pipeline& pipe) : m_pipeline(pipe) {
		m_textures = new std::vector<Texture*>();
		m_textures->reserve(32);
	}
	
	~Scene() {
		m_textures->clear();
		delete m_textures;
	}
	/**
	* Accessor method for the texture for this scene.
	* 
	* @param texFile The file to read the image from.
	*/
	virtual void setTexture(const Texture* tex, unsigned index)
	{
		if(index >= m_textures->size() || m_textures->empty()) m_textures->resize(index+1);
		
		if(m_textures->at(index) != NULL){
			delete m_textures->at(index);
		}
		(*m_textures)[index] = (Texture*)tex;
	}
	
	/**
	* Returns a reference to the texture for this scene.
	* 
	* @return reference to the current texture unit
	*/
	virtual Texture& getTexture(unsigned index)
	{
		return *(m_textures->at(index));
	}

	/**
	* Initializes buffers within the pipeline before rendering
	* 
	*/
	virtual void init() = 0;
	
	/**
	* The primary function to render the scene.
	* 
	*/
	virtual void render() = 0;

protected:
	std::vector<Texture*>* m_textures;	//!< The list of texture data for a scene. Might be empty.
	Pipeline& m_pipeline;				//!< Reference to the pipeline for dispatching render calls

};	// class Scene

class SceneSpheres : public Scene {
public:
	SceneSpheres(Pipeline& pipe) : Scene(pipe)
	{
		m_depth = 3;
		
		m_colorA = Color3f(0.4f, 0.5f, 0.8f);
		m_colorB = Color3f(0.8f, 0.5f, 0.4f);

		m_locationA = Vector3f(2.2f, 0.0f, 0.0f);
		m_locationB = Vector3f(-2.4f, 0.0f, 0.0f);
	}
	
	~SceneSpheres() {}
	
	virtual void init()
	{
		if(!m_textures->empty()){
			Texture* image0 = m_textures->at(0);
			Texture* image1 = m_textures->at(1);
			tex0 = m_pipeline.generateTexture();
			m_pipeline.bindTexture(tex0);
			m_pipeline.loadTexture2D(image0->width(), image0->height(), PIXEL_FORMAT_RGB, PIXEL_TYPE_UNSIGNED_BYTE, image0->getTextureBytes());
			tex1 = m_pipeline.generateTexture();
			m_pipeline.bindTexture(tex1);
			m_pipeline.loadTexture2D(image1->width(), image1->height(), PIXEL_FORMAT_RGB, PIXEL_TYPE_UNSIGNED_BYTE, image1->getTextureBytes());
		}
	}
	
	virtual void render() 
	{
		m_pipeline.setMatrixMode(MATRIX_MODELVIEW);
		
		if(!m_textures->empty()) m_pipeline.bindTexture(tex0);
		m_pipeline.translate(m_locationA);
	    Geometry::sphere(m_depth, m_colorA, m_pipeline);

		if(!m_textures->empty()) m_pipeline.bindTexture(tex1);
		m_pipeline.translate(m_locationB);
		m_pipeline.pushMatrix();
		m_pipeline.scale(Vector3f(0.4f, 0.5f, 0.8f));
	    Geometry::sphere(m_depth, m_colorB, m_pipeline);
		m_pipeline.popMatrix();
	}
	
protected:
	unsigned tex0;			//!< The first texture to be bound
	unsigned tex1;			//!< The second texture to be bound
	int m_depth;			//!< The triangulation depth of the spheres
	Color3f m_colorA;		//!< The color of the first sphere.
	Color3f m_colorB;		//!< The color of the second sphere.
	Vector3f m_locationA;	//!< The amount to translate the center of the first sphere.
	Vector3f m_locationB;	//!< The amount to translate the center of the second sphere. 

};	// class SceneSpheres


class SceneCube : public Scene {

public:
	SceneCube(Pipeline& pipe) : Scene(pipe) {}
	~SceneCube() {};
	
	virtual void init()
	{
		if(!m_textures->empty()){
			Texture* image0 = m_textures->at(0);
			tex0 = m_pipeline.generateTexture();
			m_pipeline.bindTexture(tex0);
			m_pipeline.loadTexture2D(image0->width(), image0->height(), PIXEL_FORMAT_RGB, PIXEL_TYPE_UNSIGNED_BYTE, image0->getTextureBytes());
		}
	}
	
	virtual void render() 
	{
		if(!m_textures->empty()) m_pipeline.bindTexture(tex0);
		Geometry::cube(m_pipeline);
	}
	
protected:
	unsigned tex0;

};	// class SceneCube

}	// namespace pixelpipe


/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, pixelpipe::Scene& s)
{
	return out << "[ Scene ]";
}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, pixelpipe::SceneSpheres& s)
{
	return out << "[ SceneSpheres ]";
}

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, pixelpipe::SceneCube& s)
{
	return out << "[ SceneCube ]";
}

#endif	// __PIPELINE_SCENE_H