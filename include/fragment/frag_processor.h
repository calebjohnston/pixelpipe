#ifndef __PIPELINE_FRAGMENT_PROCESSOR_H
#define __PIPELINE_FRAGMENT_PROCESSOR_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/texture.h"

namespace pipeline {

/**
 * A fragment processor describes what happens in order to render fragments of a
 * triangle on screen. Things that happen here include shading calculations, z
 * buffering, texturing, etc.
 * 
 * @author ags
 */
class FragmentProcessor {
public:
	
	/** A list of valid Fragment Processors */
	// static public Class[] classes = {
	// 	TrivialColorFP.class,
	// 	ColorZBufferFP.class,
	// 	TexturedFP.class,
	// 	PhongShadedFP.class,
	// 	TexturedPhongFP.class,
	// 	ReflectionMapFP.class
	// 	};
	
	/**
	 * This sets the texture that the fragment processor should use.
	 */
	void setTexture(Texture newTexture) { texture = newTexture; }
	
protected:
	Texture texture;	// A reference to the currently loaded texture.
	
	virtual int nAttr() = 0;
	virtual void fragment(const Fragment& f, FrameBuffer& fb) = 0;
	
}

#endif	// __PIPELINE_FRAGMENT_PROCESSOR_H