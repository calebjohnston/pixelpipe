
#include "core/common.h"
#include "core/pipeline.h"

using namespace cg::vecmath;

namespace pipeline {
	
Pipeline* Pipeline::instance = NULL;

/**
 * The default constructor needs to know what the dimensions are for its
 * framebuffer.
 * 
 * @param nx The width of the frame buffer.
 * @param ny The height of the frame buffer.
 */
Pipeline::Pipeline(int nx, int ny, std::vector<PointLight>* lights)
{
	framebuffer = new FrameBuffer(nx, ny);
	// configure(TrivialColorFP.class, ConstColorVP.class);
	this->lights = lights;
	
	mode = PIPELINE_MODE_NONE;
	modelviewMatrix.identity();
	projectionMatrix.identity();
	viewportMatrix.identity();
	ambientIntensity = 0.1;
	specularColor.set(0.4, 0.4, 0.4);
	specularExponent = 40.0;
	
	clipper = new Clipper(3);
	
	//Pipeline::instance = this;
	
	// EMPTY_CLASS_ARRAY = new Class[0];
	// EMPTY_OBJECT_ARRAY = new Object[0];
}

Pipeline::~Pipeline()
{
}

Pipeline* Pipeline::getInstance() {
	if(instance==NULL){
		instance = new pipeline::Pipeline();
	}
	return instance;
}


void Pipeline::setFragmentProcessor(const FragmentProcessor* fragProc)
{
	fp = const_cast<FragmentProcessor*>(fragProc);
	rasterizer = new Rasterizer(fp->nAttr(), framebuffer->getWidth(), framebuffer->getHeight());
}

void Pipeline::setVertexProcessor(const VertexProcessor* vertProc)
{
	vp = const_cast<VertexProcessor*>(vertProc);
}


/**
 * Configures the pipeline so that the triangle and fragment processors are
 * now up to date. Forces some reinitialization in order to set up things like
 * the clipper and the rasterizer.
 * 
 * @param fpClass The class of the new fragment shader.
 * @param vpClass The class of the new triangle shader.
 */
/*
void Pipeline::configure(Class fpClass, Class vpClass)
{
	// try {
		// TO-DO: MOST OF THIS STUFF WON'T WORK, FIX IT
		Constructor c = fpClass.getConstructor(EMPTY_CLASS_ARRAY);
		fp = (FragmentProcessor) c.newInstance(EMPTY_OBJECT_ARRAY);
		rasterizer = new Rasterizer(fp->nAttr(), framebuffer->getWidth(), framebuffer->getHeight());
		clipper = new Clipper(fp->nAttr());
		c = vpClass.getConstructor(EMPTY_CLASS_ARRAY);
		vp = (VertexProcessor) c.newInstance(EMPTY_OBJECT_ARRAY);
		
//		fp->updateLightModel();
		
		vp->updateTransforms(this);
		vp->updateLightModel(this);
	// }
	// catch (Exception e) {
	// 	throw new RuntimeException(e);
	// }
}
*/

/**
 * Returns true as long as the triangle and fragment processors expect the
 * same number of attributes.
 * 
 * @return True as long as the triangle and fragment processors expect the
 *         same number of attributes.
 */
bool Pipeline::validConfiguration()
{
	return fp->nAttr() == vp->nAttr();
}

/**
 * Returns true whenever the TP is set to be a flat shaded TP.
 * 
 * @return True whenever the TP is set to be a flat shaded TP.
 */
bool Pipeline::isFlatShaded()
{
	return false;
}

/**
 * Returns the current fragment program class
 * @return the class of the triangle processor
 */
// Class Pipeline::getTriangleClass()
// {
// 	return vp->getClass();
// 	
// }

/**
 * Sets the texture for the underlying FP.
 * 
 * @param texture The new texture to use.
 */
void Pipeline::setTexture(const Texture& texture)
{
	fp->setTexture(texture);
}

/**
 * Clears the underlying framebuffer.
 */
void Pipeline::clearFrameBuffer()
{
	framebuffer->clear(0, 0, 0, 1);
}

/**
 * Returns the data in the framebuffer.
 * 
 * @return The data in the framebuffer.
 */
const char* Pipeline::getFrameData()
{
	return framebuffer->getData();
}

/**
 * Sets the modelview matrix to I, and notifies the TP of the change.
 */
void Pipeline::loadIdentity()
{
	modelviewMatrix.identity();
	recomputeMatrix();
}

/**
 * Right multiplies the model view matrix by a rotation for the given axis and
 * angle, and notifies the TP of the change.
 * 
 * @param angle The amount to rotate (in radians).
 * @param axis The axis about which to rotate.
 */
void Pipeline::rotate(float angle, const Vector3f& axis)
{
	Matrix4f temp = rotationMatrix(angle, axis, false);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * temp;
	recomputeMatrix();
}

/**
 * Right multiplies the model view matrix by a translation for the given
 * values, and notifies the TP of the change.
 * 
 * @param v The translation amount.
 */
void Pipeline::translate(const Vector3f& delta)
{
	Matrix4f temp = translationMatrix(delta);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * temp;
	recomputeMatrix();
}

/**
 * Right multiplies the model view matrix by a scale for the given values, and
 * notifies the TP of the change.
 * 
 * @param v The amount to scale by.
 */
void Pipeline::scale(const Vector3f& scale)
{
	Matrix4f temp = scalingMatrix(scale);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * temp;
	recomputeMatrix();
}

/**
 * Notifies the TP of any changes to the modelview, projection, or viewing
 * matrices.
 */
void Pipeline::recomputeMatrix()
{
	// TO-DO: compile time error for this? GCC says incompatible type form Pipline cast to int.
	vp->updateTransforms(*this);
}

/**
 * Sets the modelview matrix to be equal to the indicated viewing matrix, and
 * notifies the TP of the change.
 * 
 * @param eye The location of the eye.
 * @param target The target at which the eye is looking.
 * @param up A vector that is not parallel to (target - eye) so as to indicate
 *          which direction is up.
 */
void Pipeline::lookAt(Vector3f eye, Vector3f target, Vector3f up)
{
	Matrix4f T;
	Vector3f w;
	w = eye - target;
	w.normalize();
	Vector3f u;
	u = cross(up, w);
	u.normalize();
	Vector3f v;
	v = cross(w, u);
	T = cameraToFrame(u, v, w, eye);
	// modelviewMatrix.rightCompose(T);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * T;
	recomputeMatrix();
}

/**
 * Sets the projection matrix to represent the indicated viewing volume, and
 * notifies the TP of the change.
 * 
 * @param l The left extent of the view volume.
 * @param r The right extent of the view volume.
 * @param b The bottom extent of the view volume.
 * @param t The top extent of the view volume.
 * @param n The near plane of the view volume.
 * @param f The far plane of the view volume.
 */
void Pipeline::frustum(float l, float r, float b, float t, float n, float f)
{
	projectionMatrix.identity();
	projectionMatrix[0][0] = 2 * n / (r - l);
	projectionMatrix[0][2] = (r + l) / (r - l);
	projectionMatrix[1][1] = 2 * n / (t - b);
	projectionMatrix[1][3] = (t + b) / (t - b);
	projectionMatrix[2][2] = -(f + n) / (f - n);
	projectionMatrix[2][3] = -2 * f * n / (f - n);
	projectionMatrix[3][2] = -1;
	projectionMatrix[3][3] = 0;
	recomputeMatrix();
}

/**
 * Sets the viewport matrix to the indicated window on screen, and notifies
 * the TP of the change.
 * 
 * @param x The x location of the window.
 * @param y The y location of the window.
 * @param w The width of the window.
 * @param h The height of the window.
 */
void Pipeline::viewport(int x, int y, int w, int h)
{
	float cx = x + 0.5 * w, cy = y + 0.5 * h;
	viewportMatrix.identity();
	viewportMatrix[0][0] = 0.5 * w;
	viewportMatrix[0][3] = cx;
	viewportMatrix[1][1] = 0.5 * h;
	viewportMatrix[1][3] = cy;
	viewportMatrix[2][2] = 0.5;
	viewportMatrix[2][3] = 0.5;
	recomputeMatrix();
	
}

/**
 * Sets the pipeline mode to render a particular type of primitive.
 */
void Pipeline::begin(int primType)
{
	mode = primType;
	vertexIndex = 0;
	stripParity = 0;
}

void Pipeline::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t)
{
	vp->vertex(v, c, n, t, vertexCache[vertexIndex]);
	//DEV() << "Pipeline::vertex // 0=" << vertexCache[0].v << ", 1=" << vertexCache[1].v << ", 2=" << vertexCache[2].v;
	
	switch (mode) {
	case TRIANGLES:
		if (vertexIndex == 2) {
			renderTriangle(vertexCache);
			vertexIndex = 0;
		} else
			vertexIndex++;
		break;
		
	case TRIANGLE_STRIP:
		if (vertexIndex == 2) {
			renderTriangle(vertexCache);
			swap(vertexCache, stripParity, 2);
			stripParity ^= 1;
		} else
			vertexIndex++;
		break;
		
	case TRIANGLE_FAN:
		if (vertexIndex == 2) {
			renderTriangle(vertexCache);
			swap(vertexCache, 1, 2);
		} else
			vertexIndex++;
		break;
		
	case QUADS:
		if (vertexIndex == 3) {
			renderTriangle(vertexCache);
			swap(vertexCache, 1, 2);
			swap(vertexCache, 2, 3);
			renderTriangle(vertexCache);
			vertexIndex = 0;
		} else
			vertexIndex++;
		break;
		
	case QUAD_STRIP:
		if (vertexIndex == 3) {
			swap(vertexCache, 2, 3);
			renderTriangle(vertexCache);
			swap(vertexCache, 1, 2);
			swap(vertexCache, 2, 3);
			renderTriangle(vertexCache);
			swap(vertexCache, 0, 2);
			vertexIndex = 2;
		} else
			vertexIndex++;
		break;
	}
}

void Pipeline::end()
{
	mode = PIPELINE_MODE_NONE;
}

void Pipeline::swap(Vertex* va, int i, int j) const
{
	Vertex temp = va[i];
	va[i] = va[j];
	va[j] = temp;
}

/**
 * Renders a triangle to the software pipeline.
 * 
 * @param v The 3 vertices of the triangle.
 * @param c The 3 colors of the triangle - one for each vertex.
 * @param n The 3 normals of the triangle - one for each vertex.
 * @param t The 3 texture coordinates of the triangle - one for each vertex.
 */
void Pipeline::renderTriangle(const Vector3f* v, const Color3f* c, const Vector3f* n, const Vector2f* t)
{
	DEV() << "Pipeline::renderTriangle // v,c,n,t";
	// Send to TP, get back attributes to interpolate
	vp->triangle(v, c, n, t, vertexCache);
	
	renderTriangle(vertexCache);
}

/**
 * Renders a triangle from already-processed vertices.
 * 
 * @param v The 3 vertices of the triangle.
 * @param c The 3 colors of the triangle - one for each vertex.
 * @param n The 3 normals of the triangle - one for each vertex.
 * @param t The 3 texture coordinates of the triangle - one for each vertex.
 */
void Pipeline::renderTriangle(const Vertex* vertices)
{
	// See how many "unclipped" triangles we have
	int numberOfTriangles = clipper->clip(vertices, triangle1, triangle2);
	
	// If we have none, just stop
	if (numberOfTriangles == 0) return;
	
	// If we have two...render the second one
	if (numberOfTriangles == 2) {
		// Rasterize triangle, sending results to fp
		rasterizer->rasterize(triangle2, *fp, *framebuffer);
	}
	
	// And if we have 1 or 2, render the first one
	
	//Rasterize triangle, sending results to fp
	rasterizer->rasterize(triangle1, *fp, *framebuffer);
}
	
}	// namespace pipeline
