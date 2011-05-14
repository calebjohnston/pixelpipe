#ifndef __CG_COLOR_H
#define __CG_COLOR_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/vec4.hpp"

namespace cg {
namespace vecmath {

	// RGB colors have components R,G,B in [0,1]
	typedef TVec3<float> rgbColor;
	// HSV colors should have components:
	//    H in [0,360]
	//    S in [0,1]
	//    V in [0,1]
	typedef TVec3<float> hsvColor;
	typedef TVec3<float> yiqColor;
	typedef TVec3<float> xyzColor;
	typedef TVec2<float> xyChromaticity;
	
	extern hsvColor RGBtoHSV(const rgbColor& rgb);
	extern rgbColor HSVtoRGB(const hsvColor& hsv);
	
	extern float rgb_luminance_ntsc(const rgbColor& rgb);
	extern float rgb_luminance_alt(const rgbColor& rgb);
	
	// WON'T COMPILE, WTF?
	extern yiqColor RGBtoYIQ(const rgbColor& rgb);	
	extern xyzColor RGBtoXYZ(const rgbColor& rgb);
	extern rgbColor XYZtoRGB(const xyzColor& xyz);
	extern xyChromaticity xyz_chromaticity(const xyzColor& xyz);

}	// namespace vecmath
}	// namespace cg

#endif	// __CG_COLOR_H
