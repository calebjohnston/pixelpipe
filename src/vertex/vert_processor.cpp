#include "vertex/vert_processor.h"

using namespace cg::vecmath;

namespace pixelpipe {

void VertexProcessor::updateTransforms(const SoftwarePipeline& pipe) {
	modelViewMatrix = pipe.modelviewMatrix;		
	MVP = modelViewMatrix;
	Matrix4f temp = modelViewMatrix;
	MVP = pipe.projectionMatrix * temp;
	temp = MVP;
	MVP = pipe.viewportMatrix * temp;
}

}