#include "core/appwindow.h"
#include "logger/logger.h"

namespace pipeline {

ApplicationWindow::ApplicationWindow(std::string title, int width, int height)
	: GlutWindow(title, width, height)
{
}

ApplicationWindow::~ApplicationWindow()
{
}	

int ApplicationWindow::run()
{	
	return GlutWindow::run();
}

void ApplicationWindow::init()
{
	GlutWindow::init();
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
}

int ApplicationWindow::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// swap drawing buffers
	glutSwapBuffers();

	return 0;
}

int ApplicationWindow::resize(int width, int height)
{
	GlutWindow::resize(width, height);
	
	DEV() << "resize: " << width << ", " << height;
	
	return 0;
}

int ApplicationWindow::keyPressed(unsigned char key)
{
	GlutWindow::keyPressed(key);
}

int ApplicationWindow::keyReleased(unsigned char key)
{
	DEV() << "keyReleased: " << key;
	return 0;
}

int ApplicationWindow::mousePressed(int button, int x, int y)
{
	DEV() << "mousePressed: " << x << ", " << y;
	return 0;
}

int ApplicationWindow::mouseReleased(int button, int x, int y)
{
	DEV() << "mouseReleased: " << x << ", " << y;
	return 0;
}

int ApplicationWindow::mouseMoved(int x, int y)
{
	DEV() << "mouseMoved: " << x << ", " << y;
	return 0;
}

}	// namespace pipeline