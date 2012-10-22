#ifndef __PIPELINE_GEOMETRY_H
#define __PIPELINE_GEOMETRY_H

#include <string>

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "cg/vecmath/color.h"
#include "core/pipeline.h"
#include "core/common.h"

using namespace cg::vecmath;

namespace pixelpipe {
	
	static Vector3f normals[3];
	static Color3f colors[3];
	static Vector3f vertices[3];
	static Vector2f texs[3];

	static Vector2f t0(0.0f, 0.0f);
	static Vector2f t1(1.0f, 0.0f);
	static Vector2f t2(1.0f, 1.0f);
	static Vector2f t3(0.0f, 1.0f);

	static Vector2f textures1[] = { t0, t2, t3 };
	static Vector2f textures2[] = { t2, t0, t1 };

	static Vector3f nnn(-1.0f, -1.0f, -1.0f);
	static Vector3f nnp(-1.0f, -1.0f, +1.0f);
	static Vector3f npn(-1.0f, +1.0f, -1.0f);
	static Vector3f npp(-1.0f, +1.0f, +1.0f);
	static Vector3f pnn(+1.0f, -1.0f, -1.0f);
	static Vector3f pnp(+1.0f, -1.0f, +1.0f);
	static Vector3f ppn(+1.0f, +1.0f, -1.0f);
	static Vector3f ppp(+1.0f, +1.0f, +1.0f);

	static Vector3f lNormal(-1, 0, 0);
	static Vector3f rNormal(+1, 0, 0);
	static Vector3f dNormal(0, -1, 0);
	static Vector3f uNormal(0, +1, 0);
	static Vector3f bNormal(0, 0, -1);
	static Vector3f fNormal(0, 0, +1);

	static Color3f lColor(0.4f, 0.4f, 0.8f);
	static Color3f rColor(0.8f, 0.8f, 0.4f);
	static Color3f dColor(0.8f, 0.4f, 0.4f);
	static Color3f uColor(0.4f, 0.8f, 0.8f);
	static Color3f bColor(0.8f, 0.4f, 0.8f);
	static Color3f fColor(0.4f, 0.8f, 0.4f);

	static Vector3f v_p00(+1.0f, 0.0f, 0.0f);
	static Vector3f v_n00(-1.0f, 0.0f, 0.0f);
	static Vector3f v_0p0(0.0f, +1.0f, 0.0f);
	static Vector3f v_0n0(0.0f, -1.0f, 0.0f);
	static Vector3f v_00p(0.0f, 0.0f, +1.0f);
	static Vector3f v_00n(0.0f, 0.0f, -1.0f);

class Geometry {
public:
	/**
	 * Sends a quadrilateral to the OpenGL pipeline. Mimics the other quad
	 * function.
	 */
	static void quad(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n, Color3f c, Pipeline& pipe)
	{
		pipe.begin(TRIANGLES);
		pipe.vertex(v0, c, n, t0);
		pipe.vertex(v1, c, n, t1);
		pipe.vertex(v2, c, n, t2);
		pipe.vertex(v0, c, n, t0);
		pipe.vertex(v2, c, n, t2);
		pipe.vertex(v3, c, n, t3);
		pipe.end();
	}

	/**
	 * Sends two quadrilaterals in the form of four triangles to the software
	 * pipeline. Both quads will occupy the same location, but will be back to
	 * back, giving the effect of a 2 sided polygon.
	 */
	static void quadPair(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n, Color3f c1, Color3f c2, Pipeline& pipe)
	{
		quad(v0, v1, v2, v3, n, c1, pipe);
		n *= -1.0;

		quad(v3, v2, v1, v0, n, c2, pipe);
		n *= -1.0;
	}

	/**
	 * Draws a unit plane (1x1) at the origin using the software pipeline.
	 */
	static void plane(Color3f c, Pipeline& pipe)
	{
		quad(pnn, ppn, ppp, pnp, rNormal, c, pipe);
	}

	/**
	 * Draws a unit cube (2x2x2) at the origin using the software pipeline. The
	 * colors are fixed above.
	 */
	static void cube(Pipeline& pipe)
	{
		quad(nnn, nnp, npp, npn, lNormal, lColor, pipe);
		quad(pnn, ppn, ppp, pnp, rNormal, rColor, pipe);
		quad(nnn, pnn, pnp, nnp, dNormal, dColor, pipe);
		quad(npn, npp, ppp, ppn, uNormal, uColor, pipe);
		quad(nnn, npn, ppn, pnn, bNormal, bColor, pipe);
		quad(nnp, pnp, ppp, npp, fNormal, fColor, pipe);
	}
	
	/**
	 * Draws a sphere out of triangles, using the spheretri function. 
	 */
	static void sphere(int n, Color3f c, Pipeline& pipe)
	{
		spheretri(n, v_p00, v_0p0, v_00p, c, pipe);
		spheretri(n, v_00n, v_0p0, v_p00, c, pipe);
		spheretri(n, v_n00, v_0p0, v_00n, c, pipe);
		spheretri(n, v_00p, v_0p0, v_n00, c, pipe);
		spheretri(n, v_00p, v_0n0, v_p00, c, pipe);
		spheretri(n, v_p00, v_0n0, v_00n, c, pipe);
		spheretri(n, v_00n, v_0n0, v_n00, c, pipe);
		spheretri(n, v_n00, v_0n0, v_00p, c, pipe);
	}
	
	
	/**
	 * Recursively generates a sphere using triangles and puts the resulting
	 * polygons into the software pipeline.
	 */
	static void spheretri(int n, Vector3f v0, Vector3f v1, Vector3f v2, Color3f c, Pipeline& pipe)
	{
		Vector3f nrml;
		if (n == 0) {
			vertices[0] = v0;
			vertices[1] = v1;
			vertices[2] = v2;
			colors[0] = colors[1] = colors[2] = c;
			if (pipe.isFlatShaded()) {
				nrml = v0 + v1;
				nrml += v2;
				nrml.normalize();

				normals[0] = normals[1] = normals[2] = nrml;
				
				pipe.begin(TRIANGLES);
				pipe.renderTriangle(vertices, colors, normals, NULL);
				pipe.end();
			}
			else {
				xyTex(v0, texs[0]);
				xyTex(v1, texs[1]);
				xyTex(v2, texs[2]);
				pipe.begin(TRIANGLES);
				pipe.renderTriangle(vertices, colors, vertices, texs);
				pipe.end();
			}
		}
		else {
			Vector3f v01;
			Vector3f v12;
			Vector3f v20;

			v01 = v0 + v1;
			v01.normalize();
			v12 = v1 + v2;
			v12.normalize();
			v20 = v2 + v0;
			v20.normalize();

			spheretri(n - 1, v01, v12, v20, c, pipe);
			spheretri(n - 1, v0, v01, v20, c, pipe);
			spheretri(n - 1, v1, v12, v01, c, pipe);
			spheretri(n - 1, v2, v20, v12, c, pipe);
		}
	}

	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Geometry ]";
	}
	
protected:
	
private:
	/**
	 * Takes in a 3D location and spits out its texture coordinate. This version
	 * simply returns 1/2 the x and y coordinate, offset by 0.5 This will ensure
	 * that all texture coordinates are valid, based on assumption about the
	 * incoming 3D location. This is valid because we know this method will only
	 * be called from our spheretri methods.
	 */
	static void xyTex(const Vector3f v, Vector2f& tex)
	{
		tex.x = v.x / 2 + 0.5f;
		tex.y = v.y / 2 + 0.5f;
	}
};

}

#endif	// __PIPELINE_GEOMETRY_H