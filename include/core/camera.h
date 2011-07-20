#ifndef __PIPELINE_CAMERA_H
#define __PIPELINE_CAMERA_H

#include "cg/vecmath/vec2.hpp"
#include "cg/vecmath/vec3.hpp"
#include "core/common.h"
// #include "core/pipeline.h"

namespace pipeline {
	
static const float THETA_LIMIT = (89.0 * PI / 180.0);
	
class Camera {
public:
	Camera(const cg::vecmath::Vector3f& newEye, const cg::vecmath::Vector3f& newTarget, const cg::vecmath::Vector3f& newUp, float newNear, float newFar, float newHt);
	~Camera();
	
	void orbit(const cg::vecmath::Vector2f& mouseDelta);
	void panDolly(const cg::vecmath::Vector2f& mouseDelta, bool dolly);
	void dolly(float d);
	// void setProjection(Pipeline& pipe) const;
	// void setProjection(GLAutoDrawable d);
	void setAspect(float d);
	// void setup(Pipeline& pipe) const;
	// void setup(GLAutoDrawable d);
	
	cg::vecmath::Vector3f getEye() const { return eye; }
	cg::vecmath::Vector3f getTarget() const { return target; }
	cg::vecmath::Vector3f getUp() const { return up; }
	float getNear() const { return near; }
	float getFar() const { return far; }
	float getHt() const { return ht; }
	float getAspectRatio() const { return aspect; }
	
protected:
	// GLU myGLU;

	float near;
	float far;
	float ht;
	float aspect;

	cg::vecmath::Vector3f eye;
	cg::vecmath::Vector3f target;
	cg::vecmath::Vector3f up;


private:
	static int argmin(double a, double b, double c);
	static cg::vecmath::Vector3f nonParallelVector(cg::vecmath::Vector3f v);
};

}

#endif	// __PIPELINE_CAMERA_H