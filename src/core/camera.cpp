#include <math.h>
#include "cg/vecmath/mat3.hpp"
#include "cg/vecmath/mat4.hpp"
#include "core/camera.h"
#include "core/common.h"

using namespace cg::vecmath;

namespace pixelpipe {

Camera::Camera(const Vector3f& newEye, const Vector3f& newTarget, const Vector3f& newUp, float newNear, float newFar, float newHt)
 : m_eye(newEye), m_target(newTarget), m_up(newUp), m_near(newNear), m_far(newFar), m_ht(newHt)
{
	m_aspect = 16.0/9.0;	// widescreen is the default
}

Camera::~Camera()
{
}

void Camera::orbit(const Vector2f& mouseDelta)
{
	// Build arbitrary frame at target point with w = up
	Vector3f u,v,w;
	w = m_up;
	w.normalize();
	u = this->nonParallelVector(w);
	v = cross(w, u);
	v.normalize();
	u = cross(v, w);
	Matrix3f basis;
	basis.setColumn(0,u);
	basis.setColumn(1,v);
	basis.setColumn(2,w);
	Matrix3f basisInv;
	invert(basisInv, basis);

	// write eye in that frame
	Vector3f e = m_eye;
	e -= m_target;
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
	m_eye = basis * e;
}

void Camera::panDolly(const Vector2f& mouseDelta, bool dolly)
{
	// Build frame at eye point with w up-ish and u toward target
	Vector3f u,v,w;
	
	u = m_target;
	u -= m_eye;
	Vector3f t(u);
	u.normalize();
	w = m_up;
	w.normalize();
	v = cross(w, u);
	v.normalize();
	w = cross(u, v);
	Matrix3f basis;
	basis.setColumn(0,u);
	basis.setColumn(1,v);
	basis.setColumn(2,w);
	Matrix3f basisInv;
	invert(basisInv, basis);

	// drive eye forward if dollying
	if (dolly) {
		m_eye += u * 0.1f;
	}

	// write target in that frame
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
	m_target = basis * t;
	m_target.normalize();
	m_target += m_eye;
}

void Camera::dolly(float d)
{
	m_eye *= -d;
}

void Camera::setAspect(float d)
{
	m_aspect = d;
}

int Camera::argmin(double a, double b, double c)
{
	return a < b ? (a < c ? 0 : 2) : (b < c ? 1 : 2);
}

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