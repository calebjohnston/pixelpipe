#include <algorithm>

#include "core/pixelpipe.h"
#include "core/common.h"

using namespace cg::vecmath;

namespace pixelpipe {

PixelPipeWindow::PixelPipeWindow(std::string title, int width, int height, render_mode mode)
	: GlutWindow(title, width, height)
{		
	Vector3f* eye = new Vector3f(3.0, 3.0, 3.0);
	Vector3f* target = new Vector3f(0.0, 0.0, 0.0);
	Vector3f* upVec = new Vector3f(0.0, 1.0, 0.0);
	float near = 1.0;
	float far = 1000.0;
	float ht = 0.6;
	m_camera = new Camera(*eye, *target, *upVec, near, far, ht);
	float aspect = (float)m_width / (float)m_height;
	m_camera->setAspect(aspect);
	
	Texture* tex1 = new Texture("../resources/textures/checker.png");
	Texture* tex2 = new Texture("../resources/textures/carbonite.jpg");
	// Texture* tex2 = new Texture("../resources/textures/silverblob.jpg");
	m_textures.push_back(tex1);
	m_textures.push_back(tex2);
	
	// set the textures
	m_state = State::getInstance();
	
	// set the lights
	PointLight pl1(Vector3f(-2.0, -2.0, 0), Color3f(1.0,0.5,0.5));
	PointLight pl2(Vector3f(2.0, 2.0, 0), Color3f(0.5,0.5,1.0));
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
	
	m_scene = new SceneCube(*m_pipeline);
	// m_scene = new SceneSpheres(*m_pipeline);
	
	m_state->enableLighting(true);
	m_state->enableDepthTest(true);
	m_state->enableTexturing2D(true);
	if(!m_textures.empty()){
		m_scene->setTexture(m_textures.at(0), 0);
		m_scene->setTexture(m_textures.at(1), 1);
	}
	
	m_pipeline->configure();
	
	m_scene->init();
}

int PixelPipeWindow::render() 
{	
	m_pipeline->clearFrameBuffer();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	
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
	
	m_pipeline->setMatrixMode(MATRIX_MODELVIEW);
	m_pipeline->loadIdentity();
	m_pipeline->lookAt(eye, target, up);
	
	m_scene->render();
	
    glDisable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_TEXTURE_2D);

	m_pipeline->drawFrameBuffer();
	
	return 0;
}

std::string PixelPipeWindow::getModeStr() const
{
	std::string str;		
	switch(this->m_mode){
		case RENDER_OPENGL: str = "OpenGL"; 
			break;
		case RENDER_SOFTWARE: str = "Software"; 
			break;
		case RENDER_CUDA: str = "CUDA"; 
			break;
	}
	return str;
}

int PixelPipeWindow::resize(int width, int height)
{
	GlutWindow::resize(width, height);
	
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
	Vector3f* v = NULL;
	switch(key){
		case '1': this->m_mode = RENDER_OPENGL;
			break;
	
		case '2': this->m_mode = RENDER_SOFTWARE;
			break;
	
		case '3': 
			v = new Vector3f(5.0, 2.0, 1.0);
			m_camera->setEye(*v);
			break;
	
		case '4': 
			v = new Vector3f(0.0, 4.0, 4.0);
			m_camera->setEye(*v);
			break;
	
		case '5':
			v = new Vector3f(-5.0, 0.0, -2.5);
			m_camera->setEye(*v);
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