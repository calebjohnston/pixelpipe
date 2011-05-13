
#include "core/texture.h"

namespace pipeline {

/**
 * Reads in a texture from the given file and stores the data. The image file
 * given must be stored in a format recognizable by javax.imageio.ImageIO.
 * 
 * @param imageFile The file where the texture is stored.
 */
Texture::Texture(std::string filename)
{
	// OLD...
	// try {
	// 	BufferedImage loadedImage = ImageIO.read(imageFile);
	// 
	// 	nx = loadedImage.getWidth();
	// 	ny = loadedImage.getHeight();
	// 	cData = new byte[nx * ny * 3];
	// 
	// 	System.out.print("loading " + nx + " x " + ny + " texture...");
	// 	filename = imageFile.getName();
	// 
	// 	int offset = 0;
	// 	for (int iy = 0; iy < ny; iy++) {
	// 		for (int ix = 0; ix < nx; ix++) {
	// 			int pixelValue = loadedImage.getRGB(ix, ny - 1 - iy);
	// 
	// 			cData[offset + 0] = (byte) (0xff & (pixelValue >> 16));
	// 			cData[offset + 1] = (byte) (0xff & (pixelValue >> 8));
	// 			cData[offset + 2] = (byte) (0xff & (pixelValue >> 0));
	// 
	// 			offset += 3;
	// 		}
	// 	}
	// 	cBuf = ByteBuffer.wrap(cData);
	// 
	// 	System.out.println("done.");
	// }
	// catch (IOException e) {
	// 	throw new RuntimeException(e);
	// }
	
	// NEW..
	cBuf = pipeline::read_image(filename.c_str());	// only supports TIFF, JPEG, and PNG
	std::cout << "loaded " << cBuf->width() << " x " << cBuf->height() << " texture with " << cBuf->channels() << " channels." << std::endl;
}

int Texture::width()
{
	int w = 0;
	if(cBuf!=NULL) w = cBuf->width();
	return w;
}

int Texture::height()
{
	int h = 0;
	if(cBuf!=NULL) h = cBuf->height();
	return h;
}

/**
 * Samples this texture for a given 2D location using linear interpolation. 
 * Places the result in the color parameter.
 * 
 * @param p The 2D texture coordinate.
 * @param cOut The result of sampling the texture.
 */
void Texture::sample(const float s, const float t, cg::vecmath::Color3f* cOut) const
{
	// TO-DO: Check for null input in cOut parameter...
	int nx = cBuf->width();
	int ny = cBuf->height();
	float iX = s * nx + 0.5f;
	float iY = t * ny + 0.5f;
	
	int x = (int) std::floor(iX);
	int y = (int) std::floor(iY);
	float uRatio = iX - x;
	float vRatio = iY - y;
	float uWeighted = 1.0f - uRatio;
	float vWeighted = 1.0f - vRatio;
	
	cg::vecmath::Color3f* leftTop = new cg::vecmath::Color3f();
	cg::vecmath::Color3f* rightTop = new cg::vecmath::Color3f();
	cg::vecmath::Color3f* leftBottom = new cg::vecmath::Color3f();
	cg::vecmath::Color3f* rightBottom = new cg::vecmath::Color3f();
	
	int offset = 3 * (nx * y + x);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	leftTop->set((cBuf[offset + 0] & 0xff) / 255.0f, (cBuf[offset + 1] & 0xff) / 255.0f, (cBuf[offset + 2] & 0xff) / 255.0f);
	leftTop->scale(uWeighted);
	
	offset = 3 * (nx * y + x + 1);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	rightTop->set((cBuf[offset + 0] & 0xff) / 255.0f, (cBuf[offset + 1] & 0xff) / 255.0f, (cBuf[offset + 2] & 0xff) / 255.0f);
	rightTop->scale(uRatio);
	
	rightTop->add(leftTop);
	rightTop->scale(vWeighted);
	
	offset = 3 * (nx * (y+1) + x);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	leftBottom->set((cBuf[offset + 0] & 0xff) / 255.0f, (cBuf[offset + 1] & 0xff) / 255.0f, (cBuf[offset + 2] & 0xff) / 255.0f);
	leftBottom->scale(uWeighted);
	
	offset = 3 * (nx * (y+1) + x + 1);
	if(offset >= cBuf->length()) offset = cBuf->length() - 3;
	rightBottom->set((cBuf[offset + 0] & 0xff) / 255.0f, (cBuf[offset + 1] & 0xff) / 255.0f, (cBuf[offset + 2] & 0xff) / 255.0f);
	rightBottom->scale(uRatio);
	
	rightBottom->add(leftBottom);
	rightBottom->scale(vRatio);
	
	cOut->add(rightBottom, rightTop);
}

}	// namespace pipeline