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

namespace pipeline {
	
	static Vector3f normals[3];

	static Color3f colors[3];

	static Vector3f vertices[3];
	
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

	static Vector3f nrml();

	static Vector2f texs[3];

	static Vector3f v_p00(+1.0f, 0.0f, 0.0f);
                  
	static Vector3f v_n00(-1.0f, 0.0f, 0.0f);
	              
	static Vector3f v_0p0(0.0f, +1.0f, 0.0f);
	              
	static Vector3f v_0n0(0.0f, -1.0f, 0.0f);
	              
	static Vector3f v_00p(0.0f, 0.0f, +1.0f);
	              
	static Vector3f v_00n(0.0f, 0.0f, -1.0f);

class Geometry {

public:
	/*
	* Sends a quadrilateral to the OpenGL pipeline. Mimics the other quad
	* function.
	*/
	static void quad(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n, Color3f c, bool usePipeline=false)
	{
		if(usePipeline){
			Pipeline::getInstance()->begin(TRIANGLES);
			Pipeline::getInstance()->vertex(v0, c, n, t0);
			Pipeline::getInstance()->vertex(v1, c, n, t1);
			Pipeline::getInstance()->vertex(v2, c, n, t2);
			Pipeline::getInstance()->vertex(v0, c, n, t0);
			Pipeline::getInstance()->vertex(v2, c, n, t2);
			Pipeline::getInstance()->vertex(v3, c, n, t3);
			Pipeline::getInstance()->end();
		}
		else{
			glBegin(GL_QUADS);

			glColor3f(c.x, c.y, c.z);
			glNormal3f(n.x, n.y, n.z);

			glTexCoord2f(t0.x, t0.y);
			glVertex3f(v0.x, v0.y, v0.z);

			glTexCoord2f(t1.x, t1.y);
			glVertex3f(v1.x, v1.y, v1.z);

			glTexCoord2f(t2.x, t2.y);
			glVertex3f(v2.x, v2.y, v2.z);

			glTexCoord2f(t3.x, t3.y);
			glVertex3f(v3.x, v3.y, v3.z);

			glEnd();
		}
	}

	/*
	* Sends two quadrilaterals in the form of four triangles to the software
	* pipeline. Both quads will occupy the same location, but will be back to
	* back, giving the effect of a 2 sided polygon.
	*/
	static void quadPair(Vector3f v0, Vector3f v1, Vector3f v2, Vector3f v3, Vector3f n, Color3f c1, Color3f c2, bool usePipeline=true)
	{
		quad(v0, v1, v2, v3, n, c1, usePipeline);
		//n.negate();
		n *= -1.0;

		quad(v3, v2, v1, v0, n, c2, usePipeline);
		//n.negate();
		n *= -1.0;
	}

	/*
	* Draws a unit cube (2x2x2) at the origin using the software pipeline. The
	* colors are fixed above.
	*/
	static void cube(bool usePipeline=false)
	{
		quad(nnn, nnp, npp, npn, lNormal, lColor, usePipeline);
		quad(pnn, ppn, ppp, pnp, rNormal, rColor, usePipeline);
		quad(nnn, pnn, pnp, nnp, dNormal, dColor, usePipeline);
		quad(npn, npp, ppp, ppn, uNormal, uColor, usePipeline);
		quad(nnn, npn, ppn, pnn, bNormal, bColor, usePipeline);
		quad(nnp, pnp, ppp, npp, fNormal, fColor, usePipeline);
	}
	

	/*
	* Draws a sphere out of triangles, using the spheretri function. The sphere
	* is rendered to the software pipeline.
	*/
/*
	static void sphere(int n, Color3f c)
	{
		spheretri(n, v_p00, v_0p0, v_00p, c);
		spheretri(n, v_00n, v_0p0, v_p00, c);
		spheretri(n, v_n00, v_0p0, v_00n, c);
		spheretri(n, v_00p, v_0p0, v_n00, c);
		spheretri(n, v_00p, v_0n0, v_p00, c);
		spheretri(n, v_p00, v_0n0, v_00n, c);
		spheretri(n, v_00n, v_0n0, v_n00, c);
		spheretri(n, v_n00, v_0n0, v_00p, c);
	}
*/
	/*
	* Draws a sphere out of triangles, using the spheretri function. The sphere
	* is rendered to the OpenGL pipeline. Mimics the other sphere function.
	*/
/*
	static void sphere(int n, Color3f c, GLAutoDrawable d)
	{
		spheretri(n, v_p00, v_0p0, v_00p, c, d);
		spheretri(n, v_00n, v_0p0, v_p00, c, d);
		spheretri(n, v_n00, v_0p0, v_00n, c, d);
		spheretri(n, v_00p, v_0p0, v_n00, c, d);
		spheretri(n, v_00p, v_0n0, v_p00, c, d);
		spheretri(n, v_p00, v_0n0, v_00n, c, d);
		spheretri(n, v_00n, v_0n0, v_n00, c, d);
		spheretri(n, v_n00, v_0n0, v_00p, c, d);
	}
*/
protected:
	
private:

	// /* Statically allocated variables to reduce memory allocation */
	// /* These three arrays are used by both cube and sphere methods */
	// static Vector3f normals[3];
	// 
	// static Color3f colors[3];
	// 
	// static Vector3f vertices[3];
	// 
	// 
	// /* The four texture coordinates used on all quadrilaterals */
	// static Vector2f t0;
	//                       
	// static Vector2f t1;
	//                       
	// static Vector2f t2;
	//                       
	// static Vector2f t3;
	// 
	// /* The two sets of texture coordinates used on all quadrilaterals */
	// static Vector2f textures1[];
	//                                
	// static Vector2f textures2[];
	// 
	// /* The eight vertices of the cube. n = negative, p = positive */
	// static Vector3f nnn;
	//                        
	// static Vector3f nnp;
	//                        
	// static Vector3f npn;
	//                        
	// static Vector3f npp;
	//                        
	// static Vector3f pnn;
	//                        
	// static Vector3f pnp;
	//                        
	// static Vector3f ppn;
	//                        
	// static Vector3f ppp;
	// 
	// /* Normals for the different faces of the cube */
	// static Vector3f lNormal;
	//                            
	// static Vector3f rNormal;
	//                            
	// static Vector3f dNormal;
	//                            
	// static Vector3f uNormal;
	//                            
	// static Vector3f bNormal;
	//                            
	// static Vector3f fNormal;
	// 
	// /* Colors for the different faces of the cube */
	// static Color3f lColor;
	//                          
	// static Color3f rColor;
	//                          
	// static Color3f dColor;
	//                          
	// static Color3f uColor;
	//                          
	// static Color3f bColor;
	//                          
	// static Color3f fColor;
	// 
	// /* Statically allocated arrays to reduce memory allocation overhead. */
	// /* These arrays are used by only the sphere methods. */
	// static Vector3f nrml;
	// 
	// static Vector2f texs[];
	// 
	// /* The eight initial vertices for the sphere approximation */
	// static Vector3f v_p00;
	//                          
	// static Vector3f v_n00;
	//                          
	// static Vector3f v_0p0;
	//                          
	// static Vector3f v_0n0;
	//                          
	// static Vector3f v_00p;
	//                          
	// static Vector3f v_00n;

	/*
	* Takes in a 3D location and spits out its texture coordinate. This version
	* simply returns 1/2 the x and y coordinate, offset by 0.5 This will ensure
	* that all texture coordinates are valid, based on assumption about the
	* incoming 3D location. This is valid because we know this method will only
	* be called from our spheretri methods.
	*/
	static void xyTex(Vector3f v, Vector2f tex)
	{
		tex.x = v.x / 2 + 0.5f;
		tex.y = v.y / 2 + 0.5f;
	}

  /*
   * Recursively generates a sphere using triangles and puts the resulting
   * polygons into the software pipeline.
   */
/*
  static void spheretri(int n, Vector3f v0, Vector3f v1, Vector3f v2, Color3f c) {

    if (n == 0) {
      vertices[0] = v0;
      vertices[1] = v1;
      vertices[2] = v2;
      colors[0] = colors[1] = colors[2] = c;
      if (Pipeline::getInstance()->isFlatShaded()) {
        nrml.add(v0, v1);
        nrml.add(v2);
        nrml.normalize();

        normals[0] = normals[1] = normals[2] = nrml;
        // Pipeline::getInstance()->tp.triangle(vertices, colors, normals, null);
        Pipeline::getInstance()->renderTriangle(vertices, colors, normals, null);
      }
      else {
        xyTex(v0, texs[0]);
        xyTex(v1, texs[1]);
        xyTex(v2, texs[2]);
        // Pipeline::getInstance()->tp.triangle(vertices, colors, vertices, texs);
        Pipeline::getInstance()->renderTriangle(vertices, colors, vertices, texs);
      }
    }
    else {
      Vector3f v01 = new Vector3f();
      Vector3f v12 = new Vector3f();
      Vector3f v20 = new Vector3f();

      v01.add(v0, v1);
      v01.normalize();
      v12.add(v1, v2);
      v12.normalize();
      v20.add(v2, v0);
      v20.normalize();

      spheretri(n - 1, v01, v12, v20, c);
      spheretri(n - 1, v0, v01, v20, c);
      spheretri(n - 1, v1, v12, v01, c);
      spheretri(n - 1, v2, v20, v12, c);
    }
  }
*/
  /*
   * Recursively generates a sphere using triangles and puts the resulting
   * polygons into the OpenGL pipeline. Mimics the other spheretri function.
   */
/*
  static void spheretri(int n, Vector3f v0, Vector3f v1, Vector3f v2, Color3f c, GLAutoDrawable d) {

    GL gl = d.getGL();

    if (n == 0) {
      if (Pipeline::getInstance()->isFlatShaded()) {
        nrml.add(v0, v1);
        nrml.add(v2);
        nrml.normalize();

        glBegin(GL_TRIANGLES);

        glColor3f(c.x, c.y, c.z);
        glNormal3f(nrml.x, nrml.y, nrml.z);

        glVertex3f(v0.x, v0.y, v0.z);
        glVertex3f(v1.x, v1.y, v1.z);
        glVertex3f(v2.x, v2.y, v2.z);

        glEnd();
      }
      else {
        glBegin(GL_TRIANGLES);

        glColor3f(c.x, c.y, c.z);
        xyTex(v0, texs[0]);
        xyTex(v1, texs[1]);
        xyTex(v2, texs[2]);

        glNormal3f(v0.x, v0.y, v0.z);
        glTexCoord2f(texs[0].x, texs[0].y);
        glVertex3f(v0.x, v0.y, v0.z);

        glNormal3f(v1.x, v1.y, v1.z);
        glTexCoord2f(texs[1].x, texs[1].y);
        glVertex3f(v1.x, v1.y, v1.z);

        glNormal3f(v2.x, v2.y, v2.z);
        glTexCoord2f(texs[2].x, texs[2].y);
        glVertex3f(v2.x, v2.y, v2.z);

        glEnd();
      }
    }
    else {
      Vector3f v01 = new Vector3f();
      Vector3f v12 = new Vector3f();
      Vector3f v20 = new Vector3f();

      v01.add(v0, v1);
      v01.normalize();
      v12.add(v1, v2);
      v12.normalize();
      v20.add(v2, v0);
      v20.normalize();

      spheretri(n - 1, v01, v12, v20, c, d);
      spheretri(n - 1, v0, v01, v20, c, d);
      spheretri(n - 1, v1, v12, v01, c, d);
      spheretri(n - 1, v2, v20, v12, c, d);
    }
  }
*/

};

}
/*
normals = new Vector3f[3];

colors = new Color3f[3];

vertices = new Vector3f[3];

t0 = new Vector2f(0.0f, 0.0f);

t1 = new Vector2f(1.0f, 0.0f);

t2 = new Vector2f(1.0f, 1.0f);

t3 = new Vector2f(0.0f, 1.0f);

textures1 = new Vector2f[] { t0, t2, t3 };

textures2 = new Vector2f[] { t2, t0, t1 };

nnn = new Vector3f(-1.0f, -1.0f, -1.0f);

nnp = new Vector3f(-1.0f, -1.0f, +1.0f);

npn = new Vector3f(-1.0f, +1.0f, -1.0f);

npp = new Vector3f(-1.0f, +1.0f, +1.0f);

pnn = new Vector3f(+1.0f, -1.0f, -1.0f);

pnp = new Vector3f(+1.0f, -1.0f, +1.0f);

ppn = new Vector3f(+1.0f, +1.0f, -1.0f);

ppp = new Vector3f(+1.0f, +1.0f, +1.0f);

lNormal = new Vector3f(-1, 0, 0);

rNormal = new Vector3f(+1, 0, 0);

dNormal = new Vector3f(0, -1, 0);

uNormal = new Vector3f(0, +1, 0);

bNormal = new Vector3f(0, 0, -1);

fNormal = new Vector3f(0, 0, +1);

lColor = new Color3f(0.4f, 0.4f, 0.8f);

rColor = new Color3f(0.8f, 0.8f, 0.4f);

dColor = new Color3f(0.8f, 0.4f, 0.4f);

uColor = new Color3f(0.4f, 0.8f, 0.8f);

bColor = new Color3f(0.8f, 0.4f, 0.8f);

fColor = new Color3f(0.4f, 0.8f, 0.4f);

nrml = new Vector3f();

texs = new Vector2f[] { new Vector2f(), new Vector2f(), new Vector2f() };

v_p00 = new Vector3f(+1.0f, 0.0f, 0.0f);

v_n00 = new Vector3f(-1.0f, 0.0f, 0.0f);

v_0p0 = new Vector3f(0.0f, +1.0f, 0.0f);

v_0n0 = new Vector3f(0.0f, -1.0f, 0.0f);

v_00p = new Vector3f(0.0f, 0.0f, +1.0f);

v_00n = new Vector3f(0.0f, 0.0f, -1.0f);
*/

#endif	// __PIPELINE_GEOMETRY_H