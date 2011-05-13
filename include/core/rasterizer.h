#ifndef __PIPELINE_RASTERIZER_H
#define __PIPELINE_RASTERIZER_H

#include "core/fragment.h"
#include "core/framebuffer.h"
#include "core/vertex.h"
#include "fragment/frag_processor.h"
#include "cg/vecmath/vec4.h"

namespace pipeline {

/**
 * This class is responsible for interpolating the attributes given to it across
 * the triangle, and handing off the correctly interpolated values to the
 * fragment processor. Clipping also happens within this class.
 * 
 * @author ags
 */
class Rasterizer {
public:
	Rasterizer(int newNa, int newNx, int newNy);
	~Rasterizer();
	
protected:
	int na;
	int nx;
	int ny;
	
	float** vData;
	float* xInc;
	float* yInc;
	float* rowData;
	float* pixData;
	Fragment frag;
	Vector4f posn[3];
	
	void rasterize(Vertex[] vs, FragmentProcessor& fp, FrameBuffer& fb);
	
private:
	static int ceil(float x);
	static int floor(float x);
	static float min(float a, float b, float c);
	static float max(float a, float b, float c);
	
};

}

#endif	// __PIPELINE_RASTERIZER_H