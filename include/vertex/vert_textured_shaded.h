#ifndef __PIPELINE_TEXTURED_SHADED_PROCESSOR_H
#define __PIPELINE_TEXTURED_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/vertex.h"
#include "vertex/vert_shaded_processor.h"

namespace pipeline {

/*!
 * \class TexturedShadedVP "vertex/vert_textured_shaded.h"
 * \brief Provides Texture mapping functionality that is NOT computed per-pixel.
 * 
 * This is the same vertex processor as SmoothShadedVP, but it also
 * interpolates texture coordinates, allowing for texturing to be applied
 * during the fragment stage.
 * 
 */
class TexturedShadedVP : public SmoothShadedVP {	
public:
	virtual int nAttr() const { return 5; }
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
		return out << "[ TexturedShadedVertexProcessor ]";
	}
};

}

#endif	// __PIPELINE_TEXTURED_SHADED_PROCESSOR_H
