#ifndef __PIPELINE_SCENE_H
#define __PIPELINE_SCENE_H

#include <iostream>
#include <string>

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

namespace pipeline {

/**
 * A Scene object represents a collection of geometry and texture data. A Scene
 * object should know how to render itself into both a GL context, as well as
 * the custom built context of this project framework.
 */
class Scene {
public:
	Scene() {}
	~Scene() {}
	/**
	* 
	* 
	* @param texFile The file to read the image from.
	*/
	virtual void setTexture(const Texture& tex)
	{
		*m_texture = tex;
	}
	
	/**
	* 
	* 
	* @return reference to the current texture unit
	*/
	virtual Texture& getTexture()
	{
		return *m_texture;
	}

	/**
	* 
	*/
	virtual void render(bool usePipeline=true) = 0;

protected:
	/** The texture data for a scene. Might be null. */
	Texture* m_texture;

};	// class Scene


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

	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ SceneCube ]";
	}

};	// class SceneCube

}	// namespace pipeline

#endif	// __PIPELINE_SCENE_H