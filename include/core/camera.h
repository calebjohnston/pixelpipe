#ifndef __PIPELINE_CAMERA_H
#define __PIPELINE_CAMERA_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "core/common.h"

namespace pixelpipe {
	
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
			float newNear = 0.1, 
			float newFar = 100.0, 
			float newHt = 0.1);
			
	~Camera();
	
	void orbit(const cg::vecmath::Vector2f& mouseDelta);
	void panDolly(const cg::vecmath::Vector2f& mouseDelta, bool dolly);
	void dolly(float d);
	void setAspect(float d);
	
	/**
	 * @param vec the camera's new position vector.
	 */
	void setEye(const cg::vecmath::Vector3f& vec);
	
	/**
	 * @param vec the camera's new target vector.
	 */
	void setTarget(const cg::vecmath::Vector3f& vec);
	
	/**
	 * @param vec the camera's new up vector.
	 */
	void getUp(const cg::vecmath::Vector3f& vec);
	
	/**
	 * @param val the camera's new near clipping plane distance.
	 */
	void setNear(float val);
	
	/**
	 * @param val the camera's new var clipping plane distance.
	 */
	void setFar(float val);
	
	/**
	 * @param val the camera's new height scaling factor.
	 */
	void setHt(float val);
	
	/**
	 * @param v the camera's position vector.
	 */
	cg::vecmath::Vector3f getEye() const { return m_eye; }
	
	/**
	 * @return a copy of the camera's target vector.
	 */
	cg::vecmath::Vector3f getTarget() const { return m_target; }
	
	/**
	 * @return a copy of the camera's up vector.
	 */
	cg::vecmath::Vector3f getUp() const { return m_up; }
	
	/**
	 * @return a copy of the camera's near clipping plane.
	 */
	float getNear() const { return m_near; }
	
	/**
	 * @return a copy of the camera's far clipping plane.
	 */
	float getFar() const { return m_far; }
	
	/**
	 * Accessor method for the camera's height scaling factor.
	 * 
	 * @return a copy of the ht value
	 */
	float getHt() const { return m_ht; }
	
	/**
	 * Accessor method for the camera's aspect ratio.
	 * 
	 * @return a copy of the aspect ratio.
	 */
	float getAspectRatio() const { return m_aspect; }
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ Camera ]";
	}
	
protected:
	float m_near;		//!< The depth of the near clipping plane
	float m_far;		//!< The depth of the far clipping plane
	float m_ht;			//!< The height scaling factor used on the viewport
	float m_aspect;		//!< The aspect ratio of the viewport

	cg::vecmath::Vector3f m_eye;		//!< The 3D position of the camera
	cg::vecmath::Vector3f m_target;	//!< The direction in which the camera is pointed
	cg::vecmath::Vector3f m_up;		//!< The "up" vector used to establish an orthonormal basis.

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