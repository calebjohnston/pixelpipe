
#include <string>
#include <iostream>
#include "cg/image/raster.h"

using namespace cg::image;

int main(int argc, char* argv[])
{	
	std::string filename("/Users/Caleb/Development/OpenSource/pixelpipe/resources/textures/carbonite.png");
	ByteRaster* img = read_png_image(filename.c_str());
	std::cout << "loaded " << std::endl;

    return 0;
}
