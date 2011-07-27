#ifndef __PIPELINE_SMOOTH_SHADED_PROCESSOR_H
#define __PIPELINE_SMOOTH_SHADED_PROCESSOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec4.hpp"
#include "cg/vecmath/mat4.hpp"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/pointlight.h"
#include "core/vertex.h"
#include "vertex/vert_processor.h"

namespace pipeline {

/*!
 * \class SmoothShadedVP "vertex/vert_shaded_processor.h"
 * \brief This class performs a linear interpolation on the color parameter of the vertex.
 * 
 * This triangle processor smoothly interpolates the color across the face of
 * the triangle. This is better than flat shading, but still not as nice as
 * fragment (aka phong) shading.
 * 
 */
class SmoothShadedVP : public VertexProcessor {
public:
	SmoothShadedVP();
	virtual int nAttr() { return 3; }
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
		return out << "[ SmoothShadedVertexProcessor ]";
	}
	
protected:	
	float nDotH;	//!< used for storing the dot product of the normal vector with the half vector
	float nDotL;	//!< used for storing the dot product of the normal vector with the light vector
	cg::vecmath::Matrix4f modelViewMatrix;		//!< the local model-view matrix
	cg::vecmath::Matrix4f m;					//!< the local model-view matrix
	cg::vecmath::Vector4f vert;					//!< temporary copy of the input vertex position
	cg::vecmath::Vector4f normal;				//!< temporary copy of the input vertex normal
	cg::vecmath::Vector3f viewVector;			//!< temporary copy of the view vector
	cg::vecmath::Vector3f lightVector;			//!< temporary copy of the light vector
	cg::vecmath::Vector3f halfVector;			//!< temporary copy of the half vector
	cg::vecmath::Vector3f transformedNormal;	//!< temporary copy of the matrix transformed normal vector
	cg::vecmath::Point3f transformedVertex;		//!< temporary copy of the matrix transformed vertex position
	cg::vecmath::Color3f outColor;				//!< temporary copy of the vertex color
	
};

}

#endif	// __PIPELINE_SMOOTH_SHADED_PROCESSOR_H