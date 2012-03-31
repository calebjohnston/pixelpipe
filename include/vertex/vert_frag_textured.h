#ifndef __PIPELINE_TEXTURED_FRAG_SHADED_PROCESSOR_H
#define __PIPELINE_TEXTURED_FRAG_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/vertex.h"
#include "vertex/vert_frag_shaded.h"

namespace pixelpipe {

/*!
 * \class TexturedFragmentShadedVP "vertex/vert_frag_textured.h"
 * \brief Provides UV values to the fragment processor for proper texture sampling.
 * 
 * This is the same vertex processor as FragmentShadedVP, but it also
 * interpolates texture coordinates, allowing for texturing to be done in the
 * fragment stage.
 * 
 */
class TexturedFragmentShadedVP : public FragmentShadedVP {	
public:
	TexturedFragmentShadedVP();
	virtual int nAttr() const { return size; }
	virtual void triangle(	const cg::vecmath::Vector3f* vs, 
					const cg::vecmath::Color3f* cs, 
					const cg::vecmath::Vector3f* ns_ign, 
					const cg::vecmath::Vector2f* ts_ign, 
					Vertex* output);
	virtual void vertex(const cg::vecmath::Vector3f& v, 
				const cg::vecmath::Color3f& c, 
				const cg::vecmath::Vector3f& n_ign, 
				const cg::vecmath::Vector2f& t_ign, 
				Vertex& output);

	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ TexturedFragmentShadedVertexProcessor ]";
	}
};

}

#endif	// __PIPELINE_TEXTURED_FRAG_SHADED_PROCESSOR_H
