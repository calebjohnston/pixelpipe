#include <algorithm>

#include "core/pixelpipe.h"
#include "core/common.h"
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

PixelPipeWindow::PixelPipeWindow(std::string title, int width, int height, render_mode mode)
	: GlutWindow(title, width, height)
{	
	Vector3f* eye = new Vector3f(1.0, 3.0, 3.0);
	Vector3f* target = new Vector3f(0.0, 0.0, 0.0);
	Vector3f* upVec = new Vector3f(0.0, 1.0, 0.0);
	float near = 1.0;
	float far = 1000.0;
	float ht = 0.6;
	m_camera = new Camera(*eye, *target, *upVec, near, far, ht);
	float aspect = (float)m_width / (float)m_height;
	m_camera->setAspect(aspect);
	
	//Texture* tex1 = new Texture("../resources/textures/checker.png");
	Texture* tex1 = new Texture("../resources/textures/carbonite.jpg");
	Texture* tex2 = new Texture("../resources/textures/silverblob.jpg");
	m_textures.push_back(tex1);
	m_textures.push_back(tex2);
	
	// set the textures
	m_state = State::getInstance();
	
	// set the lights
	PointLight pl1(m_camera->getEye(), Color3f(1.0,1.0,1.0));
	PointLight pl2(Vector3f(-3.0, 0.0, -5.0), Color3f(1.0,0.25,0.5));
	m_state->getLights().push_back(pl1);
	m_state->getLights().push_back(pl2);
	
	m_mode = mode;
	
	switch(m_mode){
		case RENDER_OPENGL:
			m_pipeline = new OpenGLPipeline();
			break;
		case RENDER_CUDA:
			break;
		default:
		case RENDER_SOFTWARE:
			m_pipeline = new SoftwarePipeline();
			break;
	}
}

PixelPipeWindow::~PixelPipeWindow()
{
	delete m_scene;
	delete m_pipeline;
	delete m_state;
}	

int PixelPipeWindow::run()
{	
	return GlutWindow::run();
}

void PixelPipeWindow::init()
{	
	GlutWindow::init();
	
	m_pipeline->init();
	
	switch(m_mode){
		case RENDER_OPENGL:
			this->init_openGLMode();
			break;
		case RENDER_CUDA:
			this->init_CUDAMode();
			break;
		default:
		case RENDER_SOFTWARE:
			this->init_softwareMode();
			break;
	}
	
	// m_scene = new SceneCube(*m_pipeline);
	m_scene = new SceneSpheres(*m_pipeline);
}

void PixelPipeWindow::init_softwareMode() 
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	// setup vertex shaders
	//ConstColorVP* vertProcessor = new ConstColorVP();				// 3
	SmoothShadedVP* vertProcessor = new SmoothShadedVP();			// 3
	//TexturedShadedVP* vertProcessor = new TexturedShadedVP();		// 5
	//FragmentShadedVP* vertProcessor = new FragmentShadedVP();		// 9 + 6 * lightCount
	//TexturedFragmentShadedVP* vertProcessor = new TexturedFragmentShadedVP();	// 9 + 6 * lightCount
	
	ZBufferFP* fragProcessor = new ZBufferFP();				// 3
	//ColorFP* fragProcessor = new ColorFP();					// 3
	//TexturedFP* fragProcessor = new TexturedFP();				// 5
	//PhongShadedFP* fragProcessor = new PhongShadedFP();		// 9 + 6 * lightCount
	//TexturedPhongFP* fragProcessor = new TexturedPhongFP();	// 9 + 6 * lightCount
	((SoftwarePipeline*) m_pipeline)->setVertexProcessor(vertProcessor);
	((SoftwarePipeline*) m_pipeline)->setFragmentProcessor(fragProcessor);
	//m_pipeline->setTexture(*m_textures.at(0));
	m_pipeline->configure();
}

void PixelPipeWindow::init_openGLMode()
{	
	// configure pipe.	
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glShadeModel(GL_SMOOTH);
    
	// Configure textures
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// removed this, so the specular component is modulated with the texture - not added on top.
	glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// configure lighting
	float amb[3] = { 0.1, 0.1, 0.1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    
	const unsigned max_ogl_lights = 8;
	int lights[max_ogl_lights] = { GL_LIGHT0, GL_LIGHT1, GL_LIGHT2, GL_LIGHT3, GL_LIGHT4, GL_LIGHT5, GL_LIGHT6, GL_LIGHT7 };

	for( int i = 0; i < std::min(max_ogl_lights, (unsigned) m_state->getLights().size()); i++) {
		Color3f d = m_state->getLights().at(i).getIntensity();
		Point3f p = m_state->getLights().at(i).getPosition();
		glEnable(lights[i]);
		float diffuse[4] = { d.x, d.y, d.z, 1.0 };
		float specular[4] = { 1.0, 1.0, 1.0, 1.0 };
		float position[4] = { p.x, p.y, p.z, 1.0 };
		glLightfv(lights[i], GL_DIFFUSE, diffuse);
		glLightfv(lights[i], GL_SPECULAR, specular);
		glLightfv(lights[i], GL_POSITION, position);
	}

	float mat[3] = { 0.4, 0.4, 0.4 };
	glMateriali(GL_FRONT, GL_SHININESS, 40);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat);
    
	glClearColor(0, 0, 0, 1);
}

void PixelPipeWindow::init_CUDAMode() 
{
	return;
}

int PixelPipeWindow::render()
{
	// DEV() << "PixelPipeWindow::render";
	
	switch(m_mode){
		case RENDER_OPENGL:
			this->render_openGLMode();
			break;
		case RENDER_CUDA:
			this->render_CUDAMode();
			break;
		default:
		case RENDER_SOFTWARE:
			this->render_softwareMode();
			break;
	}

	return 0;
}

void PixelPipeWindow::render_softwareMode() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// m_pipeline->projectionMatrix.identity();	// temp!
	m_pipeline->setMatrixMode(MATRIX_PROJECTION);
	m_pipeline->loadIdentity();
	
	float ht = m_camera->getHt();
	float aspect = m_camera->getAspectRatio();
	float near = m_camera->getNear();
	float far = m_camera->getFar();
	Vector3f eye = m_camera->getEye();
	Vector3f target = m_camera->getTarget();
	Vector3f up = m_camera->getUp();
	m_pipeline->frustum(-ht * aspect, ht * aspect, -ht, ht, near, far);
	
	m_pipeline->viewport(0, 0, m_width, m_height);
	
	// m_pipeline->modelviewMatrix.identity();	// temp!
	m_pipeline->setMatrixMode(MATRIX_MODELVIEW);
	m_pipeline->loadIdentity();
	
	m_pipeline->lookAt(eye, target, up);
	
	((SoftwarePipeline*) m_pipeline)->clearFrameBuffer();
	m_scene->render();
	((SoftwarePipeline*) m_pipeline)->getFrameBuffer().draw();
	
	// swap drawing buffers
	glutSwapBuffers();
}

void PixelPipeWindow::render_openGLMode() 
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float ht = m_camera->getHt();
	float aspect = m_camera->getAspectRatio();
	float near = m_camera->getNear();
	float far = m_camera->getFar();
	Vector3f eye = m_camera->getEye();
	Vector3f target = m_camera->getTarget();
	Vector3f up = m_camera->getUp();
	glFrustum(-ht * aspect, ht * aspect, -ht, ht, near, far);
	
	glViewport(0, 0, m_width, m_height);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);
	
	m_scene->render();
	
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);
	
	// swap drawing buffers
	glutSwapBuffers();
}

void PixelPipeWindow::render_CUDAMode() 
{
	return;
}

int PixelPipeWindow::resize(int width, int height)
{
	GlutWindow::resize(width, height);
	
	//DEV() << "resize: " << width << ", " << height;
	
	return 0;
}

int PixelPipeWindow::keyPressed(unsigned char key)
{
	GlutWindow::keyPressed(key);
	
	// for escape key ...
	/*
	if (key == 27) {
		die = 1;
		pthread_join(freenect_thread, NULL);
		glutDestroyWindow(window);
		free(depth_mid);
		free(depth_front);
		free(rgb_back);
		free(rgb_mid);
		free(rgb_front);
		pthread_exit(NULL);
	}
	*/
	
	switch(key){
		case '1': this->m_mode = RENDER_OPENGL;
			break;
	
		case '2': this->m_mode = RENDER_SOFTWARE;
			break;
	}
	
	return 0;
}

int PixelPipeWindow::keyReleased(unsigned char key)
{
	DEV() << "keyReleased: " << key;
	
	return 0;
}

int PixelPipeWindow::mousePressed(int button, int x, int y)
{
	DEV() << "mousePressed: " << x << ", " << y;

	m_lastMousePt->set(x, y);
	m_lastMousePt->set((2.0f * m_lastMousePt->x - m_width) / m_height, (2.0f * (m_height - m_lastMousePt->y - 1.0f) - m_height) / m_height);
			
	return 0;
}

int PixelPipeWindow::mouseReleased(int button, int x, int y)
{
	DEV() << "mouseReleased: " << x << ", " << y;
	
	return 0;
}

int PixelPipeWindow::mouseMoved(int x, int y)
{	
	m_mouseDelta->set(x,y);
 	m_mouseDelta->set((2.0f * m_mouseDelta->x - m_width) / m_height, (2.0f * (m_height - m_mouseDelta->y - 1.0) - m_height) / m_height);
	(*m_mouseDelta) -= (*m_lastMousePt);
	m_camera->orbit(*m_mouseDelta);
	m_lastMousePt->set(x,y);
	m_lastMousePt->set((2.0f * m_lastMousePt->x - m_width) / m_height, (2.0f * (m_height - m_lastMousePt->y - 1.0) - m_height) / m_height);
	
	return 0;
}

}	// namespace pixelpipe