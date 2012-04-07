#ifndef __PIPELINE_COMMON_H
#define __PIPELINE_COMMON_H

#include "logger/logger.h"
#include <boost/filesystem.hpp>

static const double PI = 3.141592653589793;


namespace pixelpipe {

enum shade_model {
	SHADE_FLAT,
	SHADE_SMOOTH
};

enum light_model {
	LIGHT_AMBIENT,
	LIGHT_LOCAL_VIEWER,
	LIGHT_COLOR_CONTROL, 
	LIGHT_TWO_SIDE
};

enum buffer_bit {
	BUFFER_COLOR,
	BUFFER_DEPTH,
	BUFFER_ACCUM,
	BUFFER_STENCIL
};

enum drawing_mode {
	TRIANGLES,
	TRIANGLE_STRIP,
	TRIANGLE_FAN,
	QUADS,
	QUAD_STRIP,
	PIPELINE_MODE_NONE
};
	
enum render_mode {
	RENDER_OPENGL,
	RENDER_SOFTWARE,
	RENDER_CUDA
};

enum matrix_mode {
	MATRIX_MODELVIEW,
	MATRIX_PROJECTION,
	MATRIX_VIEWPORT,
	MATRIX_TEXTURE,
	MATRIX_COLOR
};

}

#endif	// __PIPELINE_COMMON_H