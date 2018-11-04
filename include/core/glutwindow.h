#ifndef __PIPELINE_GLUTAPP_H
#define __PIPELINE_GLUTAPP_H

#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <thread>
#include <assert.h>
#include <math.h>

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

namespace pixelpipe {

/** the static GLUT handle for the window */	
static int window;

/** the static GLUT value for signalling an exit */
static int die;

/*!
 * \class GlutWindow "core/glutwindow.h"
 * \brief A wrapper for the GLUT I/O functionality
 * 
 * The GlutWindow class is a wrapper for GLUT callback functions. It also serves as the base 
 * class for all functionality that will be tied to a window. It also stores values useful
 * for camera manipulation.
 */
class GlutWindow {
public:
	
	/**
	 * Constructor allocates and initializes data
	 * 
	 * @param title The window title
	 * @param width the width of the window to be opened
	 * @param height the height of the window to be opened
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
	virtual bool isInitialized() { return m_initialized; }
	
	/**
	 * The init function begins the GLUT loopback
	 */
	virtual int run();

	/**
	 * The init method is used for setting the GLUT callbacks and setting up the 
	 * GLUT environment.
	 */
	virtual void init();
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	virtual inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ GlutWindow ]";
	}
	
protected:
	int m_width;						//!< The current window width
	int m_height;						//!< The current window height
	int m_lastX;						//!< The last mouse X position recorded
	int m_lastY;						//!< The last mouse Y position recorded
	int m_currentFrame;					//!< The current number of frames that have been rendered.
	bool m_running;						//!< Flag that represents whether or not the GLUT main loop is running.
	bool m_initialized;					//!< Flag that represents whether or not GLUT was initialized yet
	bool m_button1Down;					//!< Flag that represents whether or not the first mouse button is pressed.
	bool m_button2Down;					//!< Flag that represents whether or not the second mouse button is pressed.
	std::string m_windowTitle;			//!< The title of the GLUT window.
	std::mutex m_mutex;					//!< The member mutex used for any thread-sensitive data.
	cg::vecmath::Point2* m_lastMousePt;	//!< The last mouse point recorded during a click.
	cg::vecmath::Point2* m_currMousePt;	//!< The most recent mouse point recorded for a click.
	cg::vecmath::Vector2* m_mouseDelta;	//!< The difference between the last mouse click location and the new mouse click location.
	
	/**
	 * The instance method for handling a GLUT display callback.
	 * 
	 * @return the result of the function execution.
	 */
	virtual int render();
	
	/**
	 * The instance method for handling a GLUT window resize callback.
	 * 
	 * @param width the new width of the window
	 * @param height the new height of the window
	 * @return the result of the function execution.
	 */
	virtual int resize(int width, int height);
	
	/**
	 * The instance method for handling a GLUT key pressed callback.
	 * 
	 * @param the value of the key being pressed
	 * @return the result of the function execution.
	 */
	virtual int keyPressed(unsigned char key);
	
	/**
	 * The instance method for handling a GLUT key released callback.
	 * 
	 * @param the value of the key being released
	 * @return the result of the function execution.
	 */
	virtual int keyReleased(unsigned char key);
	
	/**
	 * The instance method for handling a GLUT mouse pressed callback.
	 * 
	 * @param button the integer representing which mouse button was pressed (0-2)
	 * @param x the x position of the mouse when the button was pressed
	 * @param y the y position of the mouse when the button was pressed
	 * @return the result of the function execution.
	 */
	virtual int mousePressed(int button, int x, int y);
	
	/**
	 * The instance method for handling a GLUT mouse released callback.
	 * 
	 * @param button the integer representing which mouse button was pressed (0-2)
	 * @param x the x position of the mouse when the button was pressed
	 * @param y the y position of the mouse when the button was pressed
	 * @return the result of the function execution.
	 */
	virtual int mouseReleased(int button, int x, int y);
	
	/**
	 * The instance method for handling a GLUT mouse moved callback.
	 * 
	 * @param x the x position of the mouse
	 * @param y the y position of the mouse
	 * @return the result of the function execution.
	 */
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

};	// GlutWindow

}	// namespace pixelpipe

#endif	// __PIPELINE_GLUTAPP_H