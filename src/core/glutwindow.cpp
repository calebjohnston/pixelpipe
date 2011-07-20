#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <assert.h>
#include <math.h>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/signal.hpp>

#include "logger/logger.h"
#include "core/glutwindow.h"

using namespace cg::vecmath;

namespace pipeline {
	
static GlutWindow* self;

GlutWindow::GlutWindow(std::string title, int width, int height) 
	: m_windowTitle(title), m_width(width), m_height(height)
{
	die = 0;
	m_button1Down = false;
	m_button2Down = false;
	m_lastX = 0;
	m_lastY = 0;
	m_running = false;
	
	m_lastMousePt = new Point2();
	m_currMousePt = new Point2();
	m_mouseDelta = new Vector2();

	m_currentFrame = 0;
	
	self = this;
}

GlutWindow::~GlutWindow()
{
	m_running = false;
}

int GlutWindow::run()
{
	if(!m_running){
		
		if(!m_initialized) init();
		
		m_running = true;

		m_mutex.lock();

		glutMainLoop();

		m_mutex.unlock();

		m_running = false;
	}
	
	return 0;
}

int GlutWindow::render()
{
	m_currentFrame++;
}

int GlutWindow::resize(int width, int height)
{
	m_width = width;
	m_height = height;

	glLoadIdentity();
	glViewport(0,0,m_width,m_height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1280, m_height, 0, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	
	return 0;
}

int GlutWindow::keyPressed(unsigned char key)
{
	// for escape key ...
	if (key == 27) {
		die = 1;
		glutDestroyWindow(window);
	}
	else{
		std::cout << "key pressed: " << key << std::endl;
	}
	
	return 0;
}

int GlutWindow::keyReleased(unsigned char key)
{
	return 0;
}

int GlutWindow::mousePressed(int button, int x, int y)
{
	// std::cout << "mouse pressed: " << x << ", " << y << std::endl;
	m_lastMousePt->x = x;
	m_lastMousePt->y = y;
	m_button1Down = true;
	
	return 0;
}

int GlutWindow::mouseReleased(int button, int x, int y)
{
	// std::cout << "mouse released: " << x << ", " << y << std::endl;
	m_button1Down = false;
	
	return 0;
}

int GlutWindow::mouseMoved(int x, int y)
{
	// std::cout << "mouse moved: " << x << ", " << y << std::endl;
	m_currMousePt->x = x;
	m_currMousePt->y = y;
	*m_lastMousePt = *m_currMousePt;
	
	return 0;
}

void GlutWindow::render_cb()
{	
	self->render();
}

void GlutWindow::resize_cb(int width, int height)
{
	self->resize(width,height);
}

void GlutWindow::keyPressed_cb(unsigned char key, int x, int y)
{	
	self->keyPressed(key);
}

void GlutWindow::mouse_cb(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
		self->m_button1Down = true;
		self->m_lastX = x;
		self->m_lastY = y;
		self->mousePressed(button, x, y);
	}
	else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON) {
		self->m_button1Down = false;
		self->mouseReleased(button, x, y);
	}
	else if (state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
		self->m_button2Down = true;
		self->m_lastX = x;
		self->m_lastY = y;
		self->mousePressed(button, x, y);
	}
	else if (state == GLUT_UP && button == GLUT_RIGHT_BUTTON) {
		self->m_button2Down = false;
		self->mouseReleased(button, x, y);
	}
}


void GlutWindow::motion_cb(int x, int y)
{
	if (self->m_button1Down) {
		double dx = (x - self->m_lastX)/2.0f;
		double dy = (y - self->m_lastY)/2.0f;
		self->m_lastX = x;
		self->m_lastY = y;
		glutPostRedisplay();
	}
	else if (self->m_button2Down) {
		// double dy = (y - lastY)/10.0f;
		// m_distZ -= dy;
		// m_lastY = y;
		// glutPostRedisplay();
	}
	
	self->mouseMoved(x, y);
}

void GlutWindow::init()
{	
	int argc = 1;
	char* argv = (char*) m_windowTitle.c_str();
	
	glutInit(&argc, &argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(self->m_width, self->m_height);
	glutInitWindowPosition(0, 0);

	// create X11 window
	window = glutCreateWindow(m_windowTitle.c_str());

	// pass callback handler function pointers to GLUT
	glutDisplayFunc(&render_cb);
	glutIdleFunc(&render_cb);
	glutReshapeFunc(&resize_cb);
	glutKeyboardFunc(&keyPressed_cb);
	glutMouseFunc(&mouse_cb);
	glutMotionFunc(&motion_cb);

	// init GL
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glShadeModel(GL_SMOOTH);
	
	m_initialized = true;
}

}