
#include "core/pipeline_software.h"
#include "vertex/vert_color.h"
#include "vertex/vert_frag_shaded.h"
#include "vertex/vert_frag_textured.h"
#include "vertex/vert_shaded.h"
#include "vertex/vert_textured_shaded.h"
#include "fragment/frag_color.h"
#include "fragment/frag_phong.h"
#include "fragment/frag_textured.h"
#include "fragment/frag_textured_phong.h"
#include "fragment/frag_zbuffer.h"

using namespace cg::vecmath;

namespace pixelpipe {

/**
 * The default constructor needs to know what the dimensions are for its
 * framebuffer.
 * 
 * @param nx The width of the frame buffer.
 * @param ny The height of the frame buffer.
 */
SoftwarePipeline::SoftwarePipeline(int nx, int ny)
{	
	m_framebuffer = new FrameBuffer(nx, ny);
	
	m_textureUnits = new std::vector<Texture*>();
	m_textureUnits->reserve(32);
	m_textureIndex = 0;
	
	m_mode = PIPELINE_MODE_NONE;
	m_modelviewMatrix = new Matrix4f();
	m_projectionMatrix = new Matrix4f();
	m_viewportMatrix = new Matrix4f();
	m_modelviewMatrix->identity();
	m_projectionMatrix->identity();
	m_viewportMatrix->identity();
	
	m_modelviewStack = new std::list<Matrix4f*>();
	m_modelviewStack->push_back(new Matrix4f(*m_modelviewMatrix));
	m_projectionStack = new std::list<Matrix4f*>();
	m_projectionStack->push_back(new Matrix4f(*m_projectionMatrix));
	
	m_currentMatrix = m_modelviewStack->back();
	
	m_matrixMode = MATRIX_MODELVIEW;
	
	m_clipper = new Clipper(3);
	m_rasterizer = NULL;
	m_vp = NULL;
	m_fp = NULL;
}

SoftwarePipeline::~SoftwarePipeline()
{	
	m_currentMatrix = NULL;
	
	delete m_modelviewStack;
	delete m_projectionStack;
	delete m_modelviewMatrix;
	delete m_projectionMatrix;
	delete m_viewportMatrix;
	
	if(m_vp) delete m_vp;
	if(m_fp) delete m_fp;
	if(m_clipper) delete m_clipper;
	if(m_rasterizer) delete m_rasterizer;
	if(m_framebuffer) delete m_framebuffer;
}

void SoftwarePipeline::init()
{
	m_framebuffer->init();
}

void SoftwarePipeline::setFragmentProcessor(const FragmentProcessor* fragProc)
{
	if(m_fp != NULL) delete m_fp;
	
	m_fp = const_cast<FragmentProcessor*>(fragProc);
	if(m_rasterizer==NULL) m_rasterizer = new Rasterizer(m_fp->nAttr(), m_framebuffer->width(), m_framebuffer->height());
}

void SoftwarePipeline::setVertexProcessor(const VertexProcessor* vertProc)
{
	if(m_vp != NULL) delete m_vp;
	
	m_vp = const_cast<VertexProcessor*>(vertProc);
}

void SoftwarePipeline::configure()
{
	State* state = State::getInstance();
	
	if(state->getTexturing2D()){
		if(true){
			m_vp = new TexturedFragmentShadedVP();
			m_fp = new TexturedPhongFP();
		}
		else {
			m_vp = new TexturedShadedVP();
			m_fp = new TexturedFP();
		}
	}
	else{
		if(state->getLighting()){
			m_vp = new SmoothShadedVP();
			if(state->getDepthTest()){
				m_fp = new ZBufferFP();
			}else{
				m_fp = new ColorFP();
			}
		}
		else{
			m_vp = new ConstColorVP();
			if(state->getDepthTest()){
				m_fp = new ZBufferFP();
			}else{
				m_fp = new ColorFP();
			}
		}
	}
	
	if(m_fp->nAttr() != m_vp->nAttr()) throw "Unsupported configuration.";
	
	m_rasterizer = new Rasterizer(m_fp->nAttr(), m_framebuffer->width(), m_framebuffer->height());
	m_rasterizer->setAttributeCount(m_fp->nAttr());
	m_clipper->setAttributeCount(m_fp->nAttr());
		
	m_vp->updateTransforms(*this);
	m_vp->updateLightModel(*this);
}

bool SoftwarePipeline::validConfiguration()
{
	return m_fp->nAttr() == m_vp->nAttr();
}

bool SoftwarePipeline::isFlatShaded()
{
	return false;
}

void SoftwarePipeline::clearFrameBuffer()
{
	m_framebuffer->clear(0, 0, 0, 1);
}

void SoftwarePipeline::drawFrameBuffer()
{
	m_framebuffer->draw();
	
	glutSwapBuffers();
}

const void* SoftwarePipeline::getFrameData()
{
	return (unsigned char*) m_framebuffer->getTextureBytes();
}

void SoftwarePipeline::loadIdentity()
{
	m_currentMatrix->identity();
	recomputeMatrix();
}

void SoftwarePipeline::rotate(float angle, const Vector3f& axis)
{
	Matrix4f temp = rotationMatrix(angle, axis, false);
	Matrix4f mv(*m_currentMatrix);
	(*m_currentMatrix) = mv * temp;
	recomputeMatrix();
}

void SoftwarePipeline::translate(const Vector3f& delta)
{
	Matrix4f temp = translationMatrix(delta);
	Matrix4f mv(*m_currentMatrix);
	(*m_currentMatrix) = mv * temp;
	recomputeMatrix();
}

void SoftwarePipeline::scale(const Vector3f& scale)
{
	Matrix4f temp = scalingMatrix(scale);
	Matrix4f mv(*m_currentMatrix);
	(*m_currentMatrix) = mv * temp;
	recomputeMatrix();
}

void SoftwarePipeline::recomputeMatrix()
{
	if(m_modelviewStack->size() > 1){
		m_modelviewMatrix->identity();
		std::list<Matrix4f*>::iterator iter;
		for(iter=m_modelviewStack->begin(); iter != m_modelviewStack->end(); iter++){
			(*m_modelviewMatrix) = (*m_modelviewMatrix) * (*(*iter));
		}
	}else{
		(*m_modelviewMatrix) = *(m_modelviewStack->back());
	}
	
	if(m_projectionStack->size() > 1){
		m_projectionMatrix->identity();
		std::list<Matrix4f*>::iterator iter;
		for(iter=m_projectionStack->begin(); iter != m_projectionStack->end(); iter++){
			(*m_projectionMatrix) = (*m_projectionMatrix) * (*(*iter));
		}
	}else{
		(*m_projectionMatrix) = *(m_projectionStack->back());
	}
	
	m_vp->updateTransforms(*this);
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
	Matrix4f mv(*m_currentMatrix);
	(*m_currentMatrix) = mv * T;
	recomputeMatrix();
}

void SoftwarePipeline::frustum(float l, float r, float b, float t, float n, float f)
{
	Matrix4f& mat = *m_currentMatrix;
	mat.identity();
	mat[0][0] = 2 * n / (r - l);
	mat[0][2] = (r + l) / (r - l);
	mat[1][1] = 2 * n / (t - b);
	mat[1][2] = (t + b) / (t - b);
	mat[2][2] = -(f + n) / (f - n);
	mat[2][3] = -2 * f * n / (f - n);
	mat[3][2] = -1;
	mat[3][3] = 0;
	
	recomputeMatrix();
}

void SoftwarePipeline::ortho(float l, float r, float b, float t, float n, float f)
{	
	Matrix4f& mat = *m_currentMatrix;
	mat.identity();
	mat[0][0] = 2 / (r - l);
	mat[0][3] = -(r + l) / (r - l);
	mat[1][1] = 2 / (t - b);
	mat[1][3] = -(t + b) / (t - b);
	mat[2][2] = -2 / (f - n);
	mat[2][3] = -(f + n) / (f - n);
	mat[3][3] = 1;
	
	recomputeMatrix();
}

void SoftwarePipeline::viewport(int x, int y, int w, int h)
{
	float cx = x + 0.5 * w;
	float cy = y + 0.5 * h;
	m_viewportMatrix->identity();
	(*m_viewportMatrix)[0][0] = 0.5 * w;
	(*m_viewportMatrix)[0][3] = cx;
	(*m_viewportMatrix)[1][1] = 0.5 * h;
	(*m_viewportMatrix)[1][3] = cy;
	
	recomputeMatrix();
}

void SoftwarePipeline::pushMatrix(Matrix4f* matrix)
{
	bool recompute = true;
	if(matrix==NULL){
		matrix = new Matrix4f();
		matrix->identity();
		recompute = false;
	}
	
	switch(m_matrixMode){
		case MATRIX_MODELVIEW:
			m_modelviewStack->push_back(matrix);
			m_currentMatrix = m_modelviewStack->back();
			break;
		case MATRIX_PROJECTION:
			m_projectionStack->push_back(matrix);
			m_currentMatrix = m_projectionStack->back();
			break;
		case MATRIX_VIEWPORT:
		case MATRIX_TEXTURE:
		case MATRIX_COLOR:
		default:
			throw "Unsupported operation: matrix stacks only supported for projection and modelview modes for now.";
	}
	
	if(recompute){
		recomputeMatrix();
	}
}

void SoftwarePipeline::popMatrix()
{
	switch(m_matrixMode){
		case MATRIX_MODELVIEW:
			m_modelviewStack->pop_back();
			m_currentMatrix = m_modelviewStack->back();
			break;
		case MATRIX_PROJECTION:
			m_projectionStack->pop_back();
			m_currentMatrix = m_projectionStack->back();
			break;
		case MATRIX_VIEWPORT:
		case MATRIX_TEXTURE:
		case MATRIX_COLOR:
		default:
		break;
	}
}

void SoftwarePipeline::loadMatrix(const Matrix4f& matrix)
{
	(*m_currentMatrix) = matrix;
	recomputeMatrix();
}

void SoftwarePipeline::loadTransposeMatrix(const Matrix4f& matrix)
{
	(*m_currentMatrix) = matrix;
	transpose(*m_currentMatrix);
	recomputeMatrix();
}

void SoftwarePipeline::setMatrixMode(const matrix_mode mode)
{
	m_matrixMode = mode;
	switch(m_matrixMode){
		case MATRIX_MODELVIEW:
			m_currentMatrix = m_modelviewStack->back();
			break;
		case MATRIX_PROJECTION:
			m_currentMatrix = m_projectionStack->back();
			break;
		case MATRIX_VIEWPORT:
			m_currentMatrix = m_viewportMatrix;
			break;
		case MATRIX_TEXTURE:
			// m_currentMatrix = 
			break;
		case MATRIX_COLOR:
			// m_currentMatrix = 
			break;
	}
}

void SoftwarePipeline::multiplyMatrix(const Matrix4f& matrix)
{
	(*m_currentMatrix) = (*m_currentMatrix) * matrix;
	recomputeMatrix();
}

void SoftwarePipeline::loadTransposeMatrixMultiply(const Matrix4f& matrix)
{
	Matrix4f t = transpose(matrix);
	(*m_currentMatrix) = (*m_currentMatrix) * t;
	recomputeMatrix();
}

void SoftwarePipeline::begin(const drawing_mode mode)
{
	this->m_mode = mode;
	this->m_vertexIndex = 0;
	this->m_stripParity = 0;
}

void SoftwarePipeline::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t)
{
	m_vp->vertex(v, c, n, t, m_vertexCache[m_vertexIndex]);
	
	switch (m_mode) {
	case TRIANGLES:
		if (m_vertexIndex == 2) {
			renderTriangle(m_vertexCache);
			m_vertexIndex = 0;
		} else
			m_vertexIndex++;
		break;
		
	case TRIANGLE_STRIP:
		if (m_vertexIndex == 2) {
			renderTriangle(m_vertexCache);
			swap(m_vertexCache, m_stripParity, 2);
			m_stripParity ^= 1;
		} else
			m_vertexIndex++;
		break;
		
	case TRIANGLE_FAN:
		if (m_vertexIndex == 2) {
			renderTriangle(m_vertexCache);
			swap(m_vertexCache, 1, 2);
		} else
			m_vertexIndex++;
		break;
		
	case QUADS:
		if (m_vertexIndex == 3) {
			renderTriangle(m_vertexCache);
			swap(m_vertexCache, 1, 2);
			swap(m_vertexCache, 2, 3);
			renderTriangle(m_vertexCache);
			m_vertexIndex = 0;
		} else
			m_vertexIndex++;
		break;
		
	case QUAD_STRIP:
		if (m_vertexIndex == 3) {
			swap(m_vertexCache, 2, 3);
			renderTriangle(m_vertexCache);
			swap(m_vertexCache, 1, 2);
			swap(m_vertexCache, 2, 3);
			renderTriangle(m_vertexCache);
			swap(m_vertexCache, 0, 2);
			m_vertexIndex = 2;
		} else
			m_vertexIndex++;
		break;
	}
}

Matrix4f SoftwarePipeline::getViewportMatrix()
{
	return *m_viewportMatrix;
}

Matrix4f SoftwarePipeline::getModelViewMatrix()
{
	return *m_modelviewMatrix;
}

Matrix4f SoftwarePipeline::getProjectionMatrix()
{
	return *m_projectionMatrix;
}

void SoftwarePipeline::end()
{
	m_mode = PIPELINE_MODE_NONE;
}

void SoftwarePipeline::setActiveTexture(unsigned unit)
{
	if(unit >= m_textureUnits->size()){
		throw "Invalid texture unit.";
	}
	
	m_textureIndex = unit;
}

unsigned SoftwarePipeline::getActiveTexture() const
{
	return m_textureIndex;
}

unsigned SoftwarePipeline::generateTexture()
{
	Texture* tex = new Texture();
	m_textureUnits->push_back(tex);
	return (unsigned) m_textureUnits->size()-1;
}

void SoftwarePipeline::deleteTexture(unsigned* texture)
{
	unsigned tex_offset = *texture;
	if(tex_offset >= m_textureUnits->size()){
		throw "Invalid texture unit.";
	}
	
	m_textureUnits->erase(m_textureUnits->begin() + tex_offset);
}

void SoftwarePipeline::bindTexture(unsigned texture)
{
	if(texture >= m_textureUnits->size()){
		throw "Invalid texture unit.";
	}
	
	m_textureIndex = texture;
	Texture* currentTexture = m_textureUnits->at(m_textureIndex);
	// TODO: we should verify that it's allocated here
	m_fp->setTexture(currentTexture);
}

void SoftwarePipeline::loadTexture2D(const unsigned width, const unsigned height, const pixel_format format, const pixel_type type, const void* data)
{
	int channels = 0;
	switch(format){
		case PIXEL_FORMAT_LUMINANCE:
		case PIXEL_FORMAT_LUMINANCE_ALPHA:
		case PIXEL_FORMAT_COLOR_INDEX:
		case PIXEL_FORMAT_RED:
		case PIXEL_FORMAT_GREEN:
		case PIXEL_FORMAT_BLUE:
		case PIXEL_FORMAT_ALPHA:
			channels = 1;
			break;
		case PIXEL_FORMAT_RGB:
		case PIXEL_FORMAT_BGR:
			channels = 3;
			break;
		case PIXEL_FORMAT_RGBA:
		case PIXEL_FORMAT_BGRA:
			channels = 4;
			break;
	}
	
	switch(type){
		case PIXEL_TYPE_BYTE: 
		case PIXEL_TYPE_UNSIGNED_BYTE:
		case PIXEL_TYPE_UNSIGNED_SHORT:
		case PIXEL_TYPE_UNSIGNED_INT:
		case PIXEL_TYPE_SHORT:
		case PIXEL_TYPE_INT:
			m_textureUnits->at(m_textureIndex)->setTextureData(width, height, channels, (unsigned char*) data);
			break;
		case PIXEL_TYPE_FLOAT:
		DEV() << "w=" << width << ", h=" << height << ", c=" << channels;
			m_textureUnits->at(m_textureIndex)->setTextureData(width, height, channels, (float*) data);
			// m_textureUnits->at(m_textureIndex)
			break;
	}
	
	Texture* tex1 = new Texture("../resources/textures/checker.png");
	// TODO: This should probably not happen here.
	m_fp->setTexture(tex1);
}

// TODO: Implementation incomplete
void SoftwarePipeline::clear(const buffer_bit bit)
{
	switch(bit){
		case BUFFER_DEPTH:
			// nothing yet ...
			break;
		case BUFFER_ACCUM:
			// nothing yet ...
			break;
		case BUFFER_STENCIL:
			// nothing yet ...
			break;
		default:
		case BUFFER_COLOR:
			m_framebuffer->clear(0, 0, 0, 1);
			break;
	}
}

void SoftwarePipeline::swap(Vertex* va, int i, int j) const
{
	Vertex temp = va[i];
	va[i] = va[j];
	va[j] = temp;
}

void SoftwarePipeline::renderTriangle(const Vector3f* v, const Color3f* c, const Vector3f* n, const Vector2f* t)
{
	m_vp->triangle(v, c, n, t, m_vertexCache);
	
	renderTriangle(m_vertexCache);
}

void SoftwarePipeline::renderTriangle(const Vertex* vertices)
{
	// See how many "unclipped" triangles we have
	int numberOfTriangles = m_clipper->clip(vertices, m_triangle1, m_triangle2);
	
	// If we have none, just stop
	if (numberOfTriangles == 0) return;
	
	// If we have two...render the second one
	if (numberOfTriangles == 2) {
		// Rasterize triangle, sending results to fp
		m_rasterizer->rasterize(m_triangle2, *m_fp, *m_framebuffer);
	}
	
	// And if we have 1 or 2, render the first one
	// Rasterize triangle, sending results to fp
	m_rasterizer->rasterize(m_triangle1, *m_fp, *m_framebuffer);
}
	
}	// namespace pixelpipe
