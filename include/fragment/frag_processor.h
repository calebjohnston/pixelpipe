#ifndef __PIPELINE_FRAGMENT_PROCESSOR_H
#define __PIPELINE_FRAGMENT_PROCESSOR_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/texture.h"
#include "core/state.h"

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

};	// namespace pixelpipe

/**
 * Output utility function for logging and debugging purposes.
 */
inline std::ostream& operator<<(std::ostream &out, const pixelpipe::FragmentProcessor& fp)
{
	return out << "[ FragmentProcessor ]";
}

#endif	// __PIPELINE_FRAGMENT_PROCESSOR_H