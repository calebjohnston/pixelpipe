#ifndef __PIPELINE_FRAGMENT_PROCESSOR_H
#define __PIPELINE_FRAGMENT_PROCESSOR_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/texture.h"

namespace pixelpipe {

/*!
 * \class FragmentProcessor "fragment/frag_processor.h"
 * \brief base class for all fragment processors.
 * A fragment processor describes what happens in order to render fragments of a
 * triangle on screen. Things that happen here include shading calculations, z
 * buffering, texturing, etc.
 * 
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
	
	virtual int nAttr() const = 0;
	virtual void fragment(Fragment& f, FrameBuffer& fb) = 0;
	
	/**
	 * This sets the texture that the fragment processor should use.
	 * 
	 * @param newTexture a reference to a new texture to use.
	 */
	void setTexture(const Texture& newTexture) { texture = newTexture; }
	
protected:
	Texture texture;	//!< A reference to the currently loaded texture.
	
};	// class FragmentProcessor

};	// namespace pixelpipe {

#endif	// __PIPELINE_FRAGMENT_PROCESSOR_H