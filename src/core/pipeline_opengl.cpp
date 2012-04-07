
#include "core/pipeline_opengl.h"

using namespace cg::vecmath;

namespace pixelpipe {

/**
 * 
 * 
 * @param nx The width of the frame buffer.
 * @param ny The height of the frame buffer.
 */
OpenGLPipeline::OpenGLPipeline(int nx, int ny)
{
}

OpenGLPipeline::~OpenGLPipeline()
{
}

void OpenGLPipeline::init()
{
	// framebuffer->init();
}

void OpenGLPipeline::configure()
{
	// hmm???
	
	glGenTextures(1, &(this->m_textureHandle));
	glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}

/** NOT YET IMPLEMENTED */
bool OpenGLPipeline::validConfiguration()
{	
	// return true;
	return (glGetError() == GL_NO_ERROR);
}

/** NOT YET IMPLEMENTED */
bool OpenGLPipeline::isFlatShaded()
{
	return false;
}

void OpenGLPipeline::setTexture(const Texture& texture)
{
	glEnable(GL_TEXTURE_2D);
	// glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texture.width(), texture.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, texture.getTextureBytes());
}

void OpenGLPipeline::clearFrameBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLPipeline::drawFrameBuffer()
{	
	glutSwapBuffers();
}

const char* OpenGLPipeline::getFrameData()
{
	// return framebuffer->getData();
	
	INFO() << "OpenGLPipeline::getFrameData not yet supported!";
	exit;
}

void OpenGLPipeline::loadIdentity()
{
	glLoadIdentity();
}

void OpenGLPipeline::rotate(float angle, const Vector3f& axis)
{
	glRotatef(angle, axis.x, axis.y, axis.z);
}

void OpenGLPipeline::translate(const Vector3f& delta)
{
	glTranslatef(delta.x, delta.y, delta.z);
}

void OpenGLPipeline::scale(const Vector3f& scale)
{
	glScalef(scale.x, scale.y, scale.z);
}

void OpenGLPipeline::lookAt(Vector3f eye, Vector3f target, Vector3f up)
{
	gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);
}

void OpenGLPipeline::frustum(float l, float r, float b, float t, float n, float f)
{
	glFrustum(l,r,b,t,n,f);
}

void OpenGLPipeline::ortho(float l, float r, float b, float t, float n, float f)
{
	glOrtho(l,r,b,t,n,f);
}

void OpenGLPipeline::viewport(int x, int y, int w, int h)
{
	glViewport(x,y,w,h);	
}

void OpenGLPipeline::pushMatrix(const Matrix4f* matrix)
{
	glPushMatrix();
	if(matrix != NULL){
		this->loadMatrix(*matrix);
	}
}

void OpenGLPipeline::popMatrix()
{
	glPopMatrix();
}

void OpenGLPipeline::loadMatrix(const Matrix4f& matrix)
{
	float mat[16] = { 0 };
	matrix.toArray(mat);
	glLoadMatrixf(mat);
}

void OpenGLPipeline::loadTransposeMatrix(const Matrix4f& matrix)
{	
	Matrix4f t = transpose(matrix);
	
	float mat[16] = { 0 };
	t.toArray(mat);
	glLoadTransposeMatrixf(mat);
}

void OpenGLPipeline::setMatrixMode(const matrix_mode mode)
{
	switch(mode){
		case MATRIX_MODELVIEW:
			glMatrixMode(GL_MODELVIEW);
			break;
		case MATRIX_PROJECTION:
			glMatrixMode(GL_PROJECTION);
			break;
		case MATRIX_VIEWPORT:
			// glMatrixMode(GL_MODELVIEW);
			break;
		case MATRIX_TEXTURE:
			glMatrixMode(GL_TEXTURE);
			break;
		case MATRIX_COLOR:
			glMatrixMode(GL_COLOR);
			break;
	}
}

void OpenGLPipeline::multiplyMatrix(const Matrix4f& matrix)
{
	float mat[16] = { 0 };
	matrix.toArray(mat);
	glMultMatrixf(mat);
}

void OpenGLPipeline::loadTransposeMatrixMultiply(const Matrix4f& matrix)
{
	Matrix4f t = transpose(matrix);
	
	float mat[16] = { 0 };
	t.toArray(mat);
	glMultTransposeMatrixf(mat);
}

void OpenGLPipeline::begin(const drawing_mode mode)
{
	GLenum gl_mode;
	switch(mode){
		case PIPELINE_MODE_NONE:
			// GLenum gl_mode = ???;
			break;
		case QUADS:
			gl_mode = GL_QUADS;
			break;
		case QUAD_STRIP:
			gl_mode = GL_QUAD_STRIP;
			break;
		case TRIANGLE_FAN:
			gl_mode = GL_TRIANGLE_FAN;
			break;
		case TRIANGLE_STRIP:
			gl_mode = GL_TRIANGLE_STRIP;
			break;
		default:
		case TRIANGLES:
			gl_mode = GL_TRIANGLES;
			break;
	}
	
	glBegin(gl_mode);
}

void OpenGLPipeline::vertex(const Vector3f& v, const Color3f& c, const Vector3f& n, const Vector2f& t)
{
	glColor3f(c.x, c.y, c.z);
	glNormal3f(n.x, n.y, n.z);
	glTexCoord2f(t.x, t.y);
	glVertex3f(v.x, v.y, v.z);
}

Matrix4f OpenGLPipeline::getViewportMatrix()
{
	float vp[4] = { 0 };
	float mat[16] = { 0 };
	glGetFloatv(GL_VIEWPORT, vp);
	
	float x = vp[0], y = vp[1], w = vp[2], h = vp[3];
	mat[0] = w * 0.5;
	mat[5] = h * 0.5;
	mat[10] = 1.0;
	mat[12] = x + 0.5 * w;
	mat[13] = y + 0.5 * h;
	mat[15] = 1.0;
	
	return Matrix4f(mat);
}

Matrix4f OpenGLPipeline::getModelViewMatrix()
{
	float* mat = (float*) malloc(sizeof(float)*16);
	glGetFloatv(GL_MODELVIEW_MATRIX, mat);
	return Matrix4f(mat);
}

Matrix4f OpenGLPipeline::getProjectionMatrix()
{
	float* mat = (float*) malloc(sizeof(float)*16);
	glGetFloatv(GL_PROJECTION_MATRIX, mat);
	Matrix4f matrix(mat);
	return matrix;
}

void OpenGLPipeline::end()
{
	glEnd();
}

void OpenGLPipeline::clear(const buffer_bit bit)
{
	return;
}

void OpenGLPipeline::renderTriangle(const Vector3f* v, const Color3f* c, const Vector3f* n, const Vector2f* t)
{
	for (unsigned k = 0; k < 3; k++) {
		this->vertex(v[k], c[k], n[k], t[k]);
		// glColor3f(c[k].x, c[k].y, c[k].z);
		// glNormal3f(n[k].x, n[k].y, n[k].z);
		// glTexCoord2f(t[k].x, t[k].y);
		// glVertex3f(v[k].x, v[k].y, v[k].z);
	}
}
	
}	// namespace pixelpipe
