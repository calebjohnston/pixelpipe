#include "core/pixelpipe.h"
#include "logger/logger.h"

namespace pipeline {

PixelPipeWindow::PixelPipeWindow(std::string title, int width, int height)
	: GlutWindow(title, width, height)
{
}

PixelPipeWindow::~PixelPipeWindow()
{
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
}

int PixelPipeWindow::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
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
	DEV() << "mouseMoved: " << x << ", " << y;
	return 0;
}

}	// namespace pipeline