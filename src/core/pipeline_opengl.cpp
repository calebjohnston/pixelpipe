
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
}

/** NOT YET IMPLEMENTED */
bool OpenGLPipeline::validConfiguration()
{
	return true;
}

/** NOT YET IMPLEMENTED */
bool OpenGLPipeline::isFlatShaded()
{
	return false;
}

void OpenGLPipeline::setTexture(const Texture& texture)
{
	// fp->setTexture(texture);
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
