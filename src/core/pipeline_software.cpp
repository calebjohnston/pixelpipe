
#include "core/common.h"
#include "core/pipeline_software.h"

using namespace cg::vecmath;

namespace pixelpipe {
	
Pipeline* Pipeline::instance = NULL;

/**
 * The default constructor needs to know what the dimensions are for its
 * framebuffer.
 * 
 * @param nx The width of the frame buffer.
 * @param ny The height of the frame buffer.
 */
SoftwarePipeline::SoftwarePipeline(int nx, int ny, std::vector<PointLight>* lights)
	: Pipeline(nx, ny)
{
	this->framebuffer = new FrameBuffer(nx, ny);
	// configure(TrivialColorFP.class, ConstColorVP.class);
	if(lights!=NULL){
		this->lights = lights;
	}
	else{
		this->lights = new std::vector<PointLight>();
		DEV() << "the lights: " << this->lights->size();
	}
	
	this->mode = PIPELINE_MODE_NONE;
	this->modelviewMatrix.identity();
	this->projectionMatrix.identity();
	this->viewportMatrix.identity();
	this->ambientIntensity = 0.1;
	this->specularColor.set(0.4, 0.4, 0.4);
	this->specularExponent = 40.0;
	
	this->clipper = new Clipper(3);
	
	// EMPTY_CLASS_ARRAY = new Class[0];
	// EMPTY_OBJECT_ARRAY = new Object[0];
}

SoftwarePipeline::~SoftwarePipeline()
{
}

// SoftwarePipeline* SoftwarePipeline::getInstance() {
// 	if(instance==NULL){
// 		instance = new pixelpipe::SoftwarePipeline();
// 	}
// 	return instance;
// }


void SoftwarePipeline::setFragmentProcessor(const FragmentProcessor* fragProc)
{
	fp = const_cast<FragmentProcessor*>(fragProc);
	rasterizer = new Rasterizer(fp->nAttr(), framebuffer->getWidth(), framebuffer->getHeight());
}

void SoftwarePipeline::setVertexProcessor(const VertexProcessor* vertProc)
{
	vp = const_cast<VertexProcessor*>(vertProc);
}

//void SoftwarePipeline::configure(Class fpClass, Class vpClass)
void SoftwarePipeline::configure()
{
	// try {
		// TO-DO: MOST OF THIS STUFF WON'T WORK, FIX IT
		// Constructor c = fpClass.getConstructor(EMPTY_CLASS_ARRAY);
		// fp = (FragmentProcessor) c.newInstance(EMPTY_OBJECT_ARRAY);
		// rasterizer = new Rasterizer(fp->nAttr(), framebuffer->getWidth(), framebuffer->getHeight());
		// clipper = new Clipper(fp->nAttr());
		// c = vpClass.getConstructor(EMPTY_CLASS_ARRAY);
		// vp = (VertexProcessor) c.newInstance(EMPTY_OBJECT_ARRAY);
		
//		fp->updateLightModel();
	rasterizer->setAttributeCount(fp->nAttr());
	clipper->setAttributeCount(fp->nAttr());
		
		vp->updateTransforms(*this);
		vp->updateLightModel(*this);
	// }
	// catch (Exception e) {
	// 	throw new RuntimeException(e);
	// }
}

bool SoftwarePipeline::validConfiguration()
{
	return fp->nAttr() == vp->nAttr();
}

bool SoftwarePipeline::isFlatShaded()
{
	return false;
}

void SoftwarePipeline::setTexture(const Texture& texture)
{
	fp->setTexture(texture);
}

void SoftwarePipeline::clearFrameBuffer()
{
	framebuffer->clear(0, 0, 0, 1);
}

const char* SoftwarePipeline::getFrameData()
{
	return framebuffer->getData();
}

void SoftwarePipeline::loadIdentity()
{
	modelviewMatrix.identity();
	recomputeMatrix();
}

void SoftwarePipeline::rotate(float angle, const Vector3f& axis)
{
	Matrix4f temp = rotationMatrix(angle, axis, false);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * temp;
	recomputeMatrix();
}

void SoftwarePipeline::translate(const Vector3f& delta)
{
	Matrix4f temp = translationMatrix(delta);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * temp;
	recomputeMatrix();
}

void SoftwarePipeline::scale(const Vector3f& scale)
{
	Matrix4f temp = scalingMatrix(scale);
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * temp;
	recomputeMatrix();
}

void SoftwarePipeline::recomputeMatrix()
{
	vp->updateTransforms(*this);
}

void SoftwarePipeline::lookAt(Vector3f eye, Vector3f target, Vector3f up)
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
	Matrix4f mv(modelviewMatrix);
	modelviewMatrix = mv * T;
	recomputeMatrix();
}

void SoftwarePipeline::frustum(float l, float r, float b, float t, float n, float f)
{
	projectionMatrix.identity();
	projectionMatrix[0][0] = 2 * n / (r - l);
	projectionMatrix[0][2] = (r + l) / (r - l);
	projectionMatrix[1][1] = 2 * n / (t - b);
	projectionMatrix[1][2] = (t + b) / (t - b);
	projectionMatrix[2][2] = -(f + n) / (f - n);
	projectionMatrix[2][3] = -2 * f * n / (f - n);
	projectionMatrix[3][2] = -1;
	projectionMatrix[3][3] = 0;
	recomputeMatrix();
}

void SoftwarePipeline::viewport(int x, int y, int w, int h)
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

void SoftwarePipeline::begin(int primType)
{
	mode = primType;
	vertexIndex = 0;
	stripParity = 0;
}

void SoftwarePipeline::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t)
{
	vp->vertex(v, c, n, t, vertexCache[vertexIndex]);
	
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

void SoftwarePipeline::end()
{
	mode = PIPELINE_MODE_NONE;
}

void SoftwarePipeline::renderTriangle(const Vector3f* v, const Color3f* c, const Vector3f* n, const Vector2f* t)
{
	vp->triangle(v, c, n, t, vertexCache);
	
	renderTriangle(vertexCache);
}

void SoftwarePipeline::renderTriangle(const Vertex* vertices)
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
	
}	// namespace pixelpipe
