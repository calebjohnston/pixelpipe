#include "core/pixelpipe.h"
#include "core/common.h"
#include "vertex/vert_color.h"
#include "vertex/vert_frag_shaded.h"
#include "vertex/vert_frag_textured.h"
#include "vertex/vert_shaded_processor.h"
#include "fragment/frag_color.h"
#include "fragment/frag_phong.h"
#include "fragment/frag_textured.h"
#include "fragment/frag_zbuffer.h"

using namespace cg::vecmath;

namespace pipeline {

PixelPipeWindow::PixelPipeWindow(std::string title, int width, int height)
	: GlutWindow(title, width, height)
{
	//m_scene = new SceneCube();
	m_scene = new SceneSpheres();
	
	Vector3f* eye = new Vector3f(3.0, 4.0, 5.0);
	Vector3f* target = new Vector3f(0.0, 0.0, 0.0);
	Vector3f* upVec = new Vector3f(0.0, 1.0, 0.0);
	float near = 0.1;
	float far = 1000.0;
	float ht = 0.6;
	m_camera = new Camera(*eye, *target, *upVec, near, far, ht);
	
	Texture* tex1 = new Texture("../resources/textures/checker.png");	//carbonite.jpg
	Texture* tex2 = new Texture("../resources/textures/silverblob.jpg");
	m_textures.push_back(tex1);
	m_textures.push_back(tex2);
}

PixelPipeWindow::~PixelPipeWindow()
{
	delete m_scene;
	m_pipeline = NULL;
}	

int PixelPipeWindow::run()
{	
	return GlutWindow::run();
}

void PixelPipeWindow::init()
{
	GlutWindow::init();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	
	// set the textures
	m_pipeline = Pipeline::getInstance();
	
	// set the lights
	PointLight* pl1 = new PointLight(m_camera->getEye(), Color3f(1.0,1.0,1.0));
	PointLight* pl2 = new PointLight(Vector3f(-3.0, 0.0, -5.0), Color3f(1.0,0.25,0.5));
	m_pipeline->getLights().push_back(*pl1);
	m_pipeline->getLights().push_back(*pl2);
	// delete pl1;
	// delete pl2;
	
	// set the shaders
	// ConstColorVP* vertProcessor = new ConstColorVP();
	// ColorFP* fragProcessor = new ColorFP();
	FragmentShadedVP* vertProcessor = new FragmentShadedVP();
	//PhongShadedFP* fragProcessor = new PhongShadedFP();
	TexturedFP* fragProcessor = new TexturedFP();
	m_pipeline->setVertexProcessor(vertProcessor);
	m_pipeline->setFragmentProcessor(fragProcessor);
	m_pipeline->setTexture(*m_textures.at(0));
	m_pipeline->configure();
	
	/*
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_NORMALIZE);
	glPolygonMode(GL_FRONT, GL_FILL);
	glShadeModel(GL_SMOOTH);
    
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// removed this, so the specular component is modulated with the texture - not added on top. 
	//    gl.glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	float amb[3] = { 0.1, 0.1, 0.1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, amb);
    
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    
	//setupGlLights(gl);

	//glMateriali(GL_FRONT, GL_SHININESS, 40);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, new float[] { 0.4f, 0.4f, 0.4f },0);
    
	glClearColor(0, 0, 0, 1);
    
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D);
	*/
}

int PixelPipeWindow::render()
{
	// DEV() << "PixelPipeWindow::render";
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	m_pipeline->projectionMatrix.identity();
	float ht = m_camera->getHt();
	float aspect = m_camera->getAspectRatio();
	float near = m_camera->getNear();
	float far = m_camera->getFar();
	Vector3f eye = m_camera->getEye();
	Vector3f target = m_camera->getTarget();
	Vector3f up = m_camera->getUp();
	m_pipeline->frustum(-ht * aspect, ht * aspect, -ht, ht, near, far);
	
	m_pipeline->viewport(0, 0, m_width, m_height);
	
	m_pipeline->modelviewMatrix.identity();
	m_pipeline->lookAt(eye, target, up);
	
	m_pipeline->clearFrameBuffer();
	m_scene->render(true);
	m_pipeline->getFrameBuffer().draw();


	// glEnable(GL_DEPTH_TEST);
	// glDisable(GL_TEXTURE_2D);
	// 
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// glMatrixMode(GL_MODELVIEW);
	// 
	// glMatrixMode(GL_PROJECTION);
	//     glLoadIdentity();
	// float ht = m_camera->getHt();
	// float aspect = m_camera->getAspectRatio();
	// float near = m_camera->getNear();
	// float far = m_camera->getFar();
	// Vector3f eye = m_camera->getEye();
	// Vector3f target = m_camera->getTarget();
	// Vector3f up = m_camera->getUp();
	//     glFrustum(-ht * aspect, ht * aspect, -ht, ht, near, far);
	// 
	// glViewport(0, 0, m_width, m_height);
	// 
	// glMatrixMode(GL_MODELVIEW);
	// glLoadIdentity();
	// gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, up.x, up.y, up.z);
	// 
	// m_scene->render(false);
	
	// swap drawing buffers
	glutSwapBuffers();

	return 0;
}

int PixelPipeWindow::resize(int width, int height)
{
	GlutWindow::resize(width, height);
	
	DEV() << "resize: " << width << ", " << height;
	
	return 0;
}

int PixelPipeWindow::keyPressed(unsigned char key)
{
	GlutWindow::keyPressed(key);
	//DEV() << "keyPressed: " << key;
	
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
	
	if (key == 'o') {
		_draw_depth_buffers = !_draw_depth_buffers;
	}
	*/
	
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
	return 0;
}

int PixelPipeWindow::mouseReleased(int button, int x, int y)
{
	DEV() << "mouseReleased: " << x << ", " << y;
	return 0;
}

int PixelPipeWindow::mouseMoved(int x, int y)
{
	m_currMousePt->set(x, y);
	m_currMousePt->set((2.0f * m_currMousePt->x - m_width) / m_height, (2.0f * (m_height - m_currMousePt->y - 1) - m_height) / m_height);
	
	m_mouseDelta->set(m_currMousePt->x - m_lastMousePt->x, m_currMousePt->y - m_lastMousePt->y);

	m_camera->orbit(*m_mouseDelta);
	
	*m_lastMousePt = *m_currMousePt;
	
	return 0;
}

}	// namespace pipeline