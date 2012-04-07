#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "core/rasterizer.h"
#include "core/common.h"

namespace pixelpipe {

Rasterizer::Rasterizer(int newNa, int newNx, int newNy) {
	
	m_attributes = newNa;
	m_frameWidth = newNx;
	m_frameHeight = newNy;
	
	int n = 5 + m_attributes;
	// vData is intended to be a multi-dimensional array of size [3][5+m_attributes]
	m_vData = (float*) malloc(3*n*sizeof(float));	
	m_xInc = (float*) malloc(n*sizeof(float));
	m_yInc = (float*) malloc(n*sizeof(float));
	m_rowData = (float*) malloc(n*sizeof(float));
	m_pixData = (float*) malloc(n*sizeof(float));
	
	m_frag = new Fragment(1 + m_attributes);
}

Rasterizer::~Rasterizer()
{	
	free(m_vData);
	free(m_xInc);
	free(m_yInc);
	free(m_rowData);
	free(m_pixData);
}

void Rasterizer::setAttributeCount(int count)
{
	free(m_vData);
	free(m_xInc);
	free(m_yInc);
	free(m_rowData);
	free(m_pixData);
	
	m_attributes = count;
	
	int n = 5 + count;
	// m_vData is intended to be a multi-dimensional array of size [3][5+m_attributes]
	m_vData = (float*) malloc(3*n*sizeof(float));	
	m_xInc = (float*) malloc(n*sizeof(float));
	m_yInc = (float*) malloc(n*sizeof(float));
	m_rowData = (float*) malloc(n*sizeof(float));
	m_pixData = (float*) malloc(n*sizeof(float));
	
	m_frag = new Fragment(1 + count);
}

void Rasterizer::rasterize(const Vertex* vs, FragmentProcessor& fp, FrameBuffer& fb)
{	
	// Assemble the vertex data.  Entries 0--2 are barycentric
	// coordinates; entry 3 is the screen-space depth; entries
	// 4 through 4 + (m_attributes-1) are the attributes provided in the
	// vertices; and entry 4 + m_attributes is the inverse w coordinate.
	// The caller-provided attributes are all interpolated with
	// perspective correction.
	
	cg::vecmath::Vector4f posn[3];
	int n = 5 + m_attributes;
	for (int iv=0; iv<3; iv++) {
		float invW = 1.0f / vs[iv].v.w;
		posn[iv] = vs[iv].v * invW;
		for (int k=0; k<3; k++){
			m_vData[iv*n + k] = (k == iv ? 1 : 0);
		}
		m_vData[iv*n + 3] = posn[iv].z;
		for (int ia=0; ia<m_attributes; ia++){
			m_vData[iv*n + (4 + ia)] = invW * vs[iv].attributes[ia];
		}
		m_vData[iv*n + (4 + m_attributes)] = invW;
	}
	
	// Compute the bounding box of the triangle; bail out if it is empty.
	int ixMin = std::max(0, ceil(min(posn[0].x, posn[1].x, posn[2].x)));
	int ixMax = std::min(m_frameWidth - 1, floor(max(posn[0].x, posn[1].x, posn[2].x)));
	int iyMin = std::max(0, ceil(min(posn[0].y, posn[1].y, posn[2].y)));
	int iyMax = std::min(m_frameHeight - 1, floor(max(posn[0].y, posn[1].y, posn[2].y)));
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
	for (int k = 0; k < 5 + m_attributes; k++) {
		float da1 = m_vData[1*n + k] - m_vData[0*n + k];
		float da2 = m_vData[2*n + k] - m_vData[0*n + k];
		m_xInc[k] = (da1 * dy2 - da2 * dy1) / det;
		m_yInc[k] = (da2 * dx1 - da1 * dx2) / det;
		m_rowData[k] = m_vData[0*n + k] + (ixMin - posn[0].x) * m_xInc[k] + (iyMin - posn[0].y) * m_yInc[k];
	}
	
	// Rasterize: loop over the bounding box, updating the attribute values.
	// For each pixel where the barycentric coordinates are in range, emit 
	// a fragment.  In our case this means calling the fragment processor to
	// process it immediately.
	for (m_frag->y = iyMin; m_frag->y <= iyMax; m_frag->y++) {
		for (int k = 0; k < 5 + m_attributes; k++){
			m_pixData[k] = m_rowData[k];
		}
		for (m_frag->x = ixMin; m_frag->x <= ixMax; m_frag->x++) {
			if (m_pixData[0] >= 0 && m_pixData[1] >= 0 && m_pixData[2] >= 0) {
				m_frag->attributes[0] = m_pixData[3];
				float w = 1.0f / m_pixData[4 + m_attributes];
				for (int ia = 0; ia < m_attributes; ia++){
					m_frag->attributes[1 + ia] = m_pixData[4 + ia] * w;
				}
				fp.fragment(*m_frag, fb);
			}
			for (int k = 0; k < 5 + m_attributes; k++){
				m_pixData[k] += m_xInc[k];
			}
		}
		for (int k = 0; k < 5 + m_attributes; k++){
			m_rowData[k] += m_yInc[k];
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
