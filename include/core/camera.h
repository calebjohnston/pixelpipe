#ifndef __PIPELINE_CAMERA_H
#define __PIPELINE_CAMERA_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "core/common.h"

namespace pipeline {
	
static const float THETA_LIMIT = (89.0 * PI / 180.0);	//!< a vertical limit for the camera's rotation about an object.

/*!
 * \class Camera "core/camera.h"
 * \brief The camera class simply contains all values used for managing a virtual camera.
 * 
 * The camera also provides functionality similar to what's found in GLU & OpenGL 
 * --including glFrustum and gluLookAt.
 * 
 */
class Camera {
public:
	/**
	 * Constructs a new camera using the inputs.
	 * 
	 * @param newEye the camera's position vector.
	 * @param newTarget the camera's target direction.
	 * @param newUp the camera's "up" vector.
	 * @param newNear the camera's near clipping plane.
	 * @param newFar the camera's far clipping plane.
	 * @param ht the camera's viewport height scaling factor.
	 */
	Camera(	const cg::vecmath::Vector3f& newEye, 
			const cg::vecmath::Vector3f& newTarget, 
			const cg::vecmath::Vector3f& newUp, 
			float newNear, 
			float newFar, 
			float newHt);
			
	~Camera();
	
	void orbit(const cg::vecmath::Vector2f& mouseDelta);
	void panDolly(const cg::vecmath::Vector2f& mouseDelta, bool dolly);
	void dolly(float d);
	// void setProjection(Pipeline& pipe) const;
	// void setProjection(GLAutoDrawable d);
	void setAspect(float d);
	// void setup(Pipeline& pipe) const;
	// void setup(GLAutoDrawable d);
	
	/**
	 * @param v the camera's position vector.
	 */
	cg::vecmath::Vector3f getEye() const { return eye; }
	
	/**
	 * @return a copy of the camera's target vector.
	 */
	cg::vecmath::Vector3f getTarget() const { return target; }
	
	/**
	 * @return a copy of the camera's up vector.
	 */
	cg::vecmath::Vector3f getUp() const { return up; }
	
	/**
	 * @return a copy of the camera's near clipping plane.
	 */
	float getNear() const { return near; }
	
	/**
	 * @return a copy of the camera's far clipping plane.
	 */
	float getFar() const { return far; }
	
	/**
	 * Accessor method for the camera's height scaling factor.
	 * 
	 * @return a copy of the ht value
	 */
	float getHt() const { return ht; }
	
	/**
	 * Accessor method for the camera's aspect ratio.
	 * 
	 * @return a copy of the aspect ratio.
	 */
	float getAspectRatio() const { return aspect; }
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Camera ]";
	}
	
protected:
	float near;		//!< The depth of the near clipping plane
	float far;		//!< The depth of the far clipping plane
	float ht;		//!< The height scaling factor used on the viewport
	float aspect;	//!< The aspect ratio of the viewport

	cg::vecmath::Vector3f eye;		//!< The 3D position of the camera
	cg::vecmath::Vector3f target;	//!< The direction in which the camera is pointed
	cg::vecmath::Vector3f up;		//!< The "up" vector used to establish an orthonormal basis.

private:
	
	/**
	 * Returns 0 if a is smallest, 1 if b is smallest, 2 if c is smallest.
	 */
	static int argmin(double a, double b, double c);
	
	/**
	 * Returns a vector that is not nearly parallel to v.
	 * 
	 * @param v the 3D vector to consider.
	 */
	static cg::vecmath::Vector3f nonParallelVector(cg::vecmath::Vector3f v);
};

}

#endif	// __PIPELINE_CAMERA_H