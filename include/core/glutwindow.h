#ifndef __PIPELINE_GLUTAPP_H
#define __PIPELINE_GLUTAPP_H

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <assert.h>
#include <math.h>

#include <boost/bind.hpp>
#include <boost/mem_fn.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <boost/signal.hpp>

#include "cg/vecmath/vec2.hpp"
#include "core/camera.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#endif

namespace pipeline {
	
static int window;
static int die;

/**
 * The GlutApp class is a wrapper for GLUT callback functions. It also serves as the base 
 * class for all functionality that will be tied to a window.
 */
class GlutWindow {
public:
	
	/**
	 * Constructor allocates and initializes data
	 * 
	 */
	GlutWindow(std::string title="Glut Window", int width=800, int height=600);
	
	/**
	 * Destructor de-allocates data
	 * 
	 */
	virtual ~GlutWindow();
	
	/**
	 * Accessor method for the window's width in pixels
	 */
	int getWidth() { return m_width; }
	
	/**
	 * Accessor method for the window's height in pixels
	 */
	int getHeight() { return m_height; }

	/**
	 * Accessor method for the initialization flag
	 */
	bool isInitialized() { return m_initialized; }
	
	/**
	 * The init function begins the GLUT loopback
	 * 
	 */
	virtual int run();

	/**
	 * The init method is used for setting the GLUT callbacks and setting up the 
	 * GLUT environment.
	 */
	virtual void init();
	
protected:

	/** The current window width */
	int m_width;
	
	/** The current window height */
	int m_height;
	
	/** The last mouse X position recorded */
	int m_lastX;
	
	/** The last mouse Y position recorded */
	int m_lastY;
	
	/** The current number of frames that have been rendered. */
	int m_currentFrame;
	
	/** Flag that represents whether or not the GLUT main loop is running. */
	bool m_running;
	
	/** Flag that represents whether or not GLUT was initialized yet */
	bool m_initialized;
	
	/** Flag that represents whether or not the first mouse button is pressed. */
	bool m_button1Down;
	
	/** Flag that represents whether or not the second mouse button is pressed. */
	bool m_button2Down;
	
	/** The title of the GLUT window. */
	std::string m_windowTitle;
	
	/** The member mutex used for any thread-sensitive data. */
	boost::mutex m_mutex;
	
	/** The last mouse point recorded during a click. */
	cg::vecmath::Point2* m_lastMousePt;
	
	/** The most recent mouse point recorded for a click. */
	cg::vecmath::Point2* m_currMousePt;
	
	/** The difference between the last mouse click location and the new mouse click location.  */
	cg::vecmath::Vector2* m_mouseDelta;
	
	virtual int render();
	virtual int resize(int width, int height);
	virtual int keyPressed(unsigned char key);
	virtual int keyReleased(unsigned char key);
	virtual int mousePressed(int button, int x, int y);
	virtual int mouseReleased(int button, int x, int y);
	virtual int mouseMoved(int x, int y);

private:
	/**
	 * The init function finds the device, and starts a thread that begins the input processing
	 * 
	 */
	static void render_cb();
	
	/**
	 * GLUT callback for window resizing
	 *
	 * @param width the new window width in pixels
	 * @param height the new window height in pixels
	 */
	static void resize_cb(int width, int height);
	
	/**
	 * Process keyboard input callback
	 *
	 * @param key the key pressed
	 * @param x 
	 * @param y 
	 */
	static void keyPressed_cb(unsigned char key, int x, int y);
	
	/**
	 * Mouse click callback function
	 * 
	 * @param button the mouse button pressed [0-2]
	 * @param x the x position where the mouse button was pressed (in pixels)
	 * @param y the y position where the mouse button was pressed (in pixels)
	 */
	static void mouse_cb(int button, int state, int x, int y);
	
	/**
	 * Mouse movement callback function
	 * 
	 * @param x the newest x position of the mouse cursor
	 * @param y the newest y position of the mouse cursor
	 */
	static void motion_cb(int x, int y);

};	// GlutApp

}	// namespace pipeline

#endif	// __PIPELINE_GLUTAPP_H