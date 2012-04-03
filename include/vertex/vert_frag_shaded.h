#ifndef __PIPELINE_FRAG_SHADED_PROCESSOR_H
#define __PIPELINE_FRAG_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"
#include "cg/vecmath/color.h"
#include "core/pointlight.h"
#include "core/vertex.h"
#include "vertex/vert_processor.h"

namespace pixelpipe {

/*!
 * \class FragmentShadedVP "vertex/vert_frag_shaded.h"
 * \brief Provides color values to the fragment shader for per-pixel shading.
 * 
 * Passes the normals and the colors of each vertex on to be rasterized, and
 * later shaded during the fragment stage of the pipeline. This results in the
 * highest quality images, but results in costly computation.
 * 
 * @author ags
 */
class FragmentShadedVP : public VertexProcessor {
public:	
	FragmentShadedVP();
	~FragmentShadedVP();
	virtual int nAttr() const { return size; }
	// virtual void updateTransforms(const SoftwarePipeline& pipe);
	virtual void triangle(	const cg::vecmath::Vector3f* vs, 
					const cg::vecmath::Color3f* cs, 
					const cg::vecmath::Vector3f* ns, 
					const cg::vecmath::Vector2f* ts, 
					Vertex* output);
	virtual void vertex(const cg::vecmath::Vector3f& v, 
				const cg::vecmath::Color3f& c, 
				const cg::vecmath::Vector3f& n, 
				const cg::vecmath::Vector2f& t, 
				Vertex& output);
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ FragmentShadedVertexProcessor ]";
	}
	
protected:
	cg::vecmath::Vector4f vert;					//!< temporary copy of the input vertex position
	cg::vecmath::Vector4f normal;				//!< temporary copy of the input vertex normal
	cg::vecmath::Vector3f viewVector;			//!< temporary copy of the viewing vector
	cg::vecmath::Vector3f lightVector;			//!< temporary copy of the light vector
	cg::vecmath::Vector3f halfVector;			//!< temporary copy of the half vector
	cg::vecmath::Vector3f transformedNormal;	//!< temporary copy of the transformed normal vector
	cg::vecmath::Point3f transformedVertex;		//!< temporary copy of the transformed vertex position
	int size;	//!< the size of the parameters that must be sent to the rasterizer.
	
};

}

#endif	// __PIPELINE_FRAG_SHADED_PROCESSOR_H