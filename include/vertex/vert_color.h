#ifndef __PIPELINE_CONSTANT_COLOR_PROCESSOR_H
#define __PIPELINE_CONSTANT_COLOR_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/mat4.hpp"
#include "cg/vecmath/color.h"
#include "core/vertex.h"
#include "vertex/vert_processor.h"

namespace pixelpipe {

/*!
 * \class ConstColorVP "vertex/vert_color.h"
 * \brief provides constant per-vertex color shading
 * The constant Color vertex processor sets each vertex up with a constant color, no 
 * matter what the viewing conditions.
 * 
 */
class ConstColorVP : public VertexProcessor {
public:	
	virtual int nAttr() const { return 3; }
	virtual void updateTransforms(const Pipeline& pipe);
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
		return out << "[ ConstantColorVertexProcessor ]";
	}
protected:
	cg::vecmath::Matrix4f mvp;	//!< this is the composed modelling, projection, and viewport matrix

};

}

#endif	// __PIPELINE_CONSTANT_COLOR_PROCESSOR_H
