#include <math.h>
#include "cg/vecmath/mat3.hpp"
#include "cg/vecmath/mat4.hpp"
#include "core/camera.h"
#include "core/common.h"

using namespace cg::vecmath;

namespace pipeline {

/**
 * Constructs a new camera. Requires all
 * 
 * @param newEye
 * @param newTarget
 * @param newUp
 * @param newNear
 * @param newFar
 * @param ht
 */
Camera::Camera(const Vector3f& newEye, const Vector3f& newTarget, const Vector3f& newUp, 
	float newNear, float newFar, float newHt)
{
	// eye.set(newEye);
	// target.set(newTarget);
	// up.set(newUp);	// instead...
	eye = newEye;
	target = newTarget;
	up = newUp;
	near = newNear;
	far = newFar;
	ht = newHt;

	// myGLU = new GLU();

	near = 1.0f;
	far = 10.0f;
	ht = 0.6f;
	aspect = 1.0f;
}

Camera::~Camera()
{
}

void Camera::orbit(const Vector2f& mouseDelta)
{
	// Build arbitrary frame at target point with w = up
	Vector3f u,v,w;
	w = up;
	normalize(w);
	u = nonParallelVector(w);
	v = cross(w, u);
	normalize(v);
	u = cross(v, w);
	Matrix3f basis;
	basis[0] = u;
	basis[1] = v;
	basis[2] = w;
	Matrix3f basisInv;
	invert(basisInv, basis);

	// write eye in that frame
	Vector3f e = eye;
	e -= target;
	// basisInv.transform(e);	// instead ...
	e = basisInv * e;

	// write e in spherical coordinates
	double r = e.length();
	double phi = std::atan2(e.y, e.x);
	double theta = std::asin(e.z / r);

	// increment phi and theta by mouse motion
	phi += -PI / 2 * mouseDelta.x;
	theta += -PI / 2 * mouseDelta.y;
	if (theta > THETA_LIMIT){
		theta = THETA_LIMIT;
	}
	if (theta < -THETA_LIMIT){
		theta = -THETA_LIMIT;
	}

	// write e back in cartesian world coords
	e.set((float) (r * std::cos(phi) * std::cos(theta)), (float) (r * std::sin(phi) * std::cos(theta)), (float) (r * std::sin(theta)));
	// basis.transform(e, eye);	// instead ...
	eye = basis * e;
}

void Camera::panDolly(const Vector2f& mouseDelta, bool dolly)
{
	// Build frame at eye point with w up-ish and u toward target
	Vector3f u,v,w;
	
	u = target;
	u -= eye;
	Vector3f t(u);
	normalize(u);
	w = up;
	normalize(w);
	v = cross(w, u);
	normalize(v);
	w = cross(u, v);
	Matrix3f basis;
	basis[0] = u;
	basis[1] = v;
	basis[2] = w;
	Matrix3f basisInv;
	invert(basisInv, basis);

	// drive eye forward if dollying
	if (dolly) {
		//eye.scaleAdd(0.1f, u, eye);	// instead ...
		eye += u * 0.1f;
	}

	// write target in that frame
	//basisInv.transform(t);	// instead ...
	t = basisInv * t;

	// write t in spherical coordinates
	double r = t.length();
	double phi = std::atan2(t.y, t.x);
	double theta = std::asin(t.z / r);

	// increment phi and theta by mouse motion
	phi += -PI / 2 * mouseDelta.x;
	theta += PI / 2 * mouseDelta.y;
	if (theta > THETA_LIMIT){
		theta = THETA_LIMIT;
	}
	if (theta < -THETA_LIMIT){
		theta = -THETA_LIMIT;
	}

	// write t back in cartesian world coords
	t.set((float) (r * std::cos(phi) * std::cos(theta)), (float) (r * std::sin(phi) * std::cos(theta)), (float) (r * std::sin(theta)));
	//basis.transform(t, target);	// instead ...
	target = basis * t;
	normalize(target);
	// target.add(eye);	// instead ...
	target += eye;
}

void Camera::dolly(float d)
{
	//eye.scaleAdd(-d, eye, eye);
	eye *= -d;
}

// void Camera::setProjection(Pipeline& pipe) const
// {
// 	pipe.frustum(-ht * aspect, ht * aspect, -ht, ht, near, far);
// }

// void Camera::setProjection(GLAutoDrawable d)
// {
// 	d.getGL().glMatrixMode(GL.GL_PROJECTION);
// 	d.getGL().glLoadIdentity();
// 	d.getGL().glFrustum(-ht * aspect, ht * aspect, -ht, ht, near, far);
// }

void Camera::setAspect(float d)
{
	aspect = d;
}

// void Camera::setup(Pipeline& pipe) const
// {
// 	pipe.lookAt(eye, target, up);
// }

// void Camera::setup(GLAutoDrawable d)
// {
// 	d.getGL().glMatrixMode(GL.GL_MODELVIEW);
// 	d.getGL().glLoadIdentity();
// 	myGLU.gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);
// }


/*
 * Returns 0 if a is smallest, 1 if b is smallest, 2 if c is smallest.
 */
int Camera::argmin(double a, double b, double c)
{
	return a < b ? (a < c ? 0 : 2) : (b < c ? 1 : 2);
}

/* Returns a vector that is not nearly parallel to v. */
Vector3f Camera::nonParallelVector(Vector3f v)
{
	int i = argmin(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	Vector3f u;
	
	if (i == 0){
		u.x = 1;
	}else if (i == 1){
		u.y = 1;
	}else if (i == 2){
		u.z = 1;
	}
	return u;
}

}
