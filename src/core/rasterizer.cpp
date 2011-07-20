#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "core/pipeline.h"
#include "core/common.h"

namespace pipeline {
	
/**
 * The only constructor.
 * 
 * @param newNa The number of user defined attributes.
 * @param newNx The width of the image.
 * @param newNy The height of the image.
 */
Rasterizer::Rasterizer(int newNa, int newNx, int newNy) {
	
	na = newNa;
	nx = newNx;
	ny = newNy;
	
	int n = 5 + na;
	// vData is intended to be a multi-dimensional array of size [3][5+na]
	vData = (float**) malloc(3*n*sizeof(float*));
	xInc = (float*) malloc(n*sizeof(float));
	yInc = (float*) malloc(n*sizeof(float));
	rowData = (float*) malloc(n*sizeof(float));
	pixData = (float*) malloc(n*sizeof(float));
	
	frag = new Fragment(1 + na);
}

Rasterizer::~Rasterizer()
{	
	free(vData);
	free(xInc);
	free(yInc);
	free(rowData);
	free(pixData);
}

void Rasterizer::rasterize(const Vertex* vs, FragmentProcessor& fp, FrameBuffer& fb)
{
	DEV() << "Rasterizer::rasterize";
	
	// Assemble the vertex data.  Entries 0--2 are barycentric
	// coordinates; entry 3 is the screen-space depth; entries
	// 4 through 4 + (na-1) are the attributes provided in the
	// vertices; and entry 4 + na is the inverse w coordinate.
	// The caller-provided attributes are all interpolated with
	// perspective correction.
	for (int iv=0; iv<3; iv++) {
		float invW = 1.0f / vs[iv].v.w;
		posn[iv] = vs[iv].v * invW;
		for (int k=0; k<3; k++){
			vData[iv][k] = (k == iv ? 1 : 0);
		}
		vData[iv][3] = posn[iv].z;
		for (int ia=0; ia<na; ia++){
			vData[iv][4 + ia] = invW * vs[iv].attributes[ia];
		}
		vData[iv][4 + na] = invW;
	}
	
	// Compute the bounding box of the triangle; bail out if it is empty.
	int ixMin = std::max(0, ceil(min(posn[0].x, posn[1].x, posn[2].x)));
	int ixMax = std::min(nx - 1, floor(max(posn[0].x, posn[1].x, posn[2].x)));
	int iyMin = std::max(0, ceil(min(posn[0].y, posn[1].y, posn[2].y)));
	int iyMax = std::min(ny - 1, floor(max(posn[0].y, posn[1].y, posn[2].y)));
	if (ixMin > ixMax || iyMin > iyMax){
		return;
	}
	
	// Compute the determinant for triangle setup.  If it is negative, the
	// triangle is back-facing and we cull it.
	float dx1 = posn[1].x - posn[0].x, dy1 = posn[1].y - posn[0].y;
	float dx2 = posn[2].x - posn[0].x, dy2 = posn[2].y - posn[0].y;
	float det = dx1 * dy2 - dx2 * dy1;
	if (det < 0){
		return;
	}
	
	// Triangle setup: compute the initial values and the x and y increments
	// for each attribute.
	for (int k = 0; k < 5 + na; k++) {
		float da1 = vData[1][k] - vData[0][k];
		float da2 = vData[2][k] - vData[0][k];
		xInc[k] = (da1 * dy2 - da2 * dy1) / det;
		yInc[k] = (da2 * dx1 - da1 * dx2) / det;
		rowData[k] = vData[0][k] + (ixMin - posn[0].x) * xInc[k] + (iyMin - posn[0].y) * yInc[k];
	}
	
	// Rasterize: loop over the bounding box, updating the attribute values.
	// For each pixel where the barycentric coordinates are in range, emit 
	// a fragment.  In our case this means calling the fragment processor to
	// process it immediately.
	for (frag->y = iyMin; frag->y <= iyMax; frag->y++) {
		for (int k = 0; k < 5 + na; k++){
			pixData[k] = rowData[k];
		}
		for (frag->x = ixMin; frag->x <= ixMax; frag->x++) {
			if (pixData[0] >= 0 && pixData[1] >= 0 && pixData[2] >= 0) {
				frag->attributes[0] = pixData[3];
				float w = 1.0f / pixData[4 + na];
				for (int ia = 0; ia < na; ia++){
					frag->attributes[1 + ia] = pixData[4 + ia] * w;
				}
				fp.fragment(*frag, fb);
			}
			for (int k = 0; k < 5 + na; k++){
				pixData[k] += xInc[k];
			}
		}
		for (int k = 0; k < 5 + na; k++){
			rowData[k] += yInc[k];
		}
	}
}


// Utility routines for clarity
int Rasterizer::ceil(float x)
{	
	return (int) std::ceil(x);
}

int Rasterizer::floor(float x)
{	
	return (int) std::floor(x);
}

float Rasterizer::min(float a, float b, float c)
{	
	return std::min(std::min(a, b), c);
}

float Rasterizer::max(float a, float b, float c)
{
	return std::max(std::max(a, b), c);
}

}
