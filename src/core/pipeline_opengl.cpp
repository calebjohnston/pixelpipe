
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
	State* state = State::getInstance();
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT, GL_FILL);
	
	if(state->getTexturing2D()){
		glEnable(GL_TEXTURE_2D);
		
		// prepare texture
		glGenTextures(1, &(this->m_textureHandle));
		glBindTexture(GL_TEXTURE_2D, this->m_textureHandle);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		
		// Configure textures
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
	}else{
		glDisable(GL_TEXTURE_2D);
	}
	
	// glShadeModel(GL_SMOOTH);
	if(state->getLighting()){
		if(state->getTexturing2D()){
			if(true){
				glDisable(GL_COLOR_MATERIAL);
				float white[3] = {1.0, 1.0, 1.0};
				glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
				glMaterialfv(GL_FRONT, GL_AMBIENT, white);
			}
			else if(false){
				glEnable(GL_COLOR_MATERIAL);
			}
			else{
	        	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
			}
		}
		
		glEnable(GL_LIGHTING);
		glShadeModel(GL_SMOOTH);
		// removed this, so the specular component is modulated with the texture - not added on top.
		glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);
		
		// configure lighting
		Color3f s = state->getSpecularColor();
		float a = state->getAmbientIntensity();
		float amb[3] = { a, a, a };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
		glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

		const unsigned max_ogl_lights = 8;
		int lights[max_ogl_lights] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

		for(size_t i = 0; i < std::min(max_ogl_lights, (unsigned) state->getLights().size()); i++) {
			Color3f d = state->getLights().at(i).getIntensity();
			Point3f p = state->getLights().at(i).getPosition();

			glEnable(lights[i]);
			float diffuse[4] = { d.x, d.y, d.z, 1.0 };
			float specular[4] = { 1.0, 1.0, 1.0, 1.0 };
			float position[4] = { p.x, p.y, p.z, 1.0 };

			glLightfv(lights[i], GL_DIFFUSE, diffuse);
			glLightfv(lights[i], GL_SPECULAR, specular);
			glLightfv(lights[i], GL_POSITION, position);
		}
		
		float mat[3] = { s.x, s.y, s.z };
		glMateriali(GL_FRONT, GL_SHININESS, state->getSpecularExponent());
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
		
	}else{
		glDisable(GL_LIGHTING);
	}
	
	if(state->getDepthTest()){
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else{
		glDisable(GL_DEPTH_TEST);
	}
		
	glClearColor(0, 0, 0, 1);
}

/** NOT YET IMPLEMENTED */
bool OpenGLPipeline::validConfiguration()
{	
	return (glGetError() == GL_NO_ERROR);
}

/** NOT YET IMPLEMENTED */
bool OpenGLPipeline::isFlatShaded()
{
	return false;
}

void OpenGLPipeline::clearFrameBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLPipeline::drawFrameBuffer()
{	
	glutSwapBuffers();
}

const void* OpenGLPipeline::getFrameData()
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

void OpenGLPipeline::pushMatrix(Matrix4f* matrix)
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

	// glPointSize(3.0f);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(gl_mode);
	// glBegin(gl_mode);
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

void OpenGLPipeline::setActiveTexture(unsigned unit)
{
	glActiveTexture(unit);
	// check for GL_INVALID_ENUM
}

unsigned OpenGLPipeline::getActiveTexture() const
{
	GLint val[1] = { 0 };
	glGetIntegerv(GL_ACTIVE_TEXTURE, val);
	return (unsigned) val[0];
}

unsigned OpenGLPipeline::generateTexture()
{
	GLuint tex[1] = { 0 };
	glGenTextures(1, tex);
	return (unsigned) tex[0];
}

void OpenGLPipeline::deleteTexture(unsigned* texture)
{
	glDeleteTextures(1, texture);
	// get error codes, look for: GL_INVALID_VALUE, GL_INVALID_OPERATION
}

void OpenGLPipeline::bindTexture(unsigned texture)
{
	if(State::getInstance()->getTexturing2D()) glEnable(GL_TEXTURE_2D);
	else glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture);
	// get error codes, look for: GL_INVALID_ENUM, GL_INVALID_OPERATION
	
	// Configure textures
	// TODO: this should be moved elsewhere, or controlled explictly
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void OpenGLPipeline::loadTexture2D(const unsigned width, const unsigned height, const pixel_format format, const pixel_type type, const void* data)
{
	GLenum pformat = GL_RGB;
	GLenum ptype = GL_UNSIGNED_BYTE;
	switch(type){
		case PIXEL_TYPE_BYTE: ptype = GL_BYTE;
			break;
		case PIXEL_TYPE_UNSIGNED_BYTE: ptype = GL_UNSIGNED_BYTE;
			break;
		case PIXEL_TYPE_UNSIGNED_SHORT: ptype = GL_UNSIGNED_SHORT;
			break;
		case PIXEL_TYPE_SHORT: ptype = GL_SHORT;
			break;
		case PIXEL_TYPE_UNSIGNED_INT: ptype = GL_UNSIGNED_INT;
			break;
		case PIXEL_TYPE_INT: ptype = GL_INT;
			break;
		case PIXEL_TYPE_FLOAT: ptype = GL_FLOAT;
			break;
	}
	
	switch(format){
		case PIXEL_FORMAT_COLOR_INDEX: pformat = GL_COLOR_INDEX;
			break;
		case PIXEL_FORMAT_RED: pformat = GL_RED;
			break;
		case PIXEL_FORMAT_GREEN: pformat = GL_GREEN;
			break;
		case PIXEL_FORMAT_BLUE: pformat = GL_BLUE;
			break;
		case PIXEL_FORMAT_ALPHA: pformat = GL_ALPHA;
			break;
		case PIXEL_FORMAT_RGB: pformat = GL_RGB;
			break;
		case PIXEL_FORMAT_BGR: pformat = GL_BGR;
			break;
		case PIXEL_FORMAT_RGBA: pformat = GL_RGBA;
			break;
		case PIXEL_FORMAT_BGRA: pformat = GL_BGRA;
			break;
		case PIXEL_FORMAT_LUMINANCE: pformat = GL_LUMINANCE;
			break;
		case PIXEL_FORMAT_LUMINANCE_ALPHA: pformat = GL_LUMINANCE_ALPHA;
			break;
	}
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, pformat, ptype, data);
	// collect error data
}

void OpenGLPipeline::clear(const buffer_bit bit)
{
	return;
}

void OpenGLPipeline::renderTriangle(const Vector3f* v, const Color3f* c, const Vector3f* n, const Vector2f* t)
{
	for (unsigned k = 0; k < 3; k++) {
		this->vertex(v[k], c[k], n[k], t[k]);
	}
}
	
}	// namespace pixelpipe
