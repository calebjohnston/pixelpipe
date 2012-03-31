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
	Scene() {
		m_texture = new std::vector<Texture*>();
		m_texture->reserve(32);
	}
	
	~Scene() {
		m_texture->clear();
		delete m_texture;
	}
	/**
	* Accessor method for the texture for this scene.
	* 
	* @param texFile The file to read the image from.
	*/
	virtual void setTexture(const Texture* tex, int index)
	{
		// for now we will rely upon vector::at to throw the out of bounds exception
		//if(m_texture->size() < index){
			if(m_texture->at(index) != NULL){
				delete m_texture->at(index);
			}
			(*m_texture)[index] = (Texture*)tex;
		//}
	}
	
	/**
	* Returns a reference to the texture for this scene.
	* 
	* @return reference to the current texture unit
	*/
	virtual Texture& getTexture(int index)
	{
		return *(m_texture->at(index));
	}

	/**
	* The primary function to render the scene.
	* 
	* @param usePipeline a flag to determine whether or not to use OpenGL rendering or the internal PixelPipe.
	*/
	virtual void render(bool usePipeline=true) = 0;

protected:
	std::vector<Texture*>* m_texture;	//!< The list of texture data for a scene. Might be empty.

};	// class Scene

class SceneSpheres : public Scene {
public:
	SceneSpheres()
	{
		depth = 3;
		
		colorA = Color3f(0.4f, 0.5f, 0.8f);
		colorB = Color3f(0.8f, 0.5f, 0.4f);

		locationA = Vector3f(1.2f, 0.0f, 0.0f);
		locationB = Vector3f(-2.4f, 0.0f, 0.0f);
	}
	
	~SceneSpheres() {}
	
	virtual void render(bool usePipeline) 
	{
		if(usePipeline){			
		    //Pipeline::getInstance()->setTexture(*m_texture);

			Pipeline::getInstance()->translate(locationA);
		    Geometry::sphere(depth, colorA, true);

			Pipeline::getInstance()->translate(locationB);
		    Geometry::sphere(depth, colorB, true);
		}
		else{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texture->width(), m_texture->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_texture->getTextureData());	// ERROR!
			// texture.cBuf;
			
		    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.nx, texture.ny, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.cBuf);
		    //texture.cBuf.rewind();

		    glTranslatef(1.2f, 0.0f, 0.0f);
		    Geometry::sphere(depth, colorA, false);

		    glTranslatef(-2.4f, 0.0f, 0.0f);
		    Geometry::sphere(depth, colorB, false);
		}
	}

	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ SceneSpheres ]";
	}
	
protected:
	int depth;			//!< The triangulation depth of the spheres
	Color3f colorA;		//!< The color of the first sphere.
	Color3f colorB;		//!< The color of the second sphere.
	Vector3f locationA;	//!< The amount to translate the center of the first sphere.
	Vector3f locationB;	//!< The amount to translate the center of the second sphere. 

};	// class SceneSpheres


class SceneCube : public Scene {

public:
	SceneCube() {};
	~SceneCube() {};
	virtual void render(bool usePipeline) 
	{
		if(usePipeline){
			//Pipeline::getInstance()->setTexture(*m_texture);
			Geometry::cube(true);
		}
		else{
			//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texture->width(), m_texture->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_texture->getTextureData());	// ERROR!
			// texture.cBuf;
			Geometry::cube(false);
		}
	}

	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ SceneCube ]";
	}

};	// class SceneCube

}	// namespace pixelpipe {

#endif	// __PIPELINE_SCENE_H