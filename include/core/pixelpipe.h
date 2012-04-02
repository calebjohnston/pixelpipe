#ifndef __PIPELINE_APP_WINDOW_H
#define __PIPELINE_APP_WINDOW_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "core/common.h"
#include "core/glutwindow.h"
#include "core/pipeline_opengl.h"
#include "core/pipeline_software.h"
#include "core/camera.h"
#include "core/geometry.h"
#include "core/scene.h"

namespace pixelpipe {

/*!
 * \class PixelPipeWindow "core/pixelpipe.h"
 * \brief This class provides the display window for the pipeline functionality.
 * 
 * This class also handles all GLUT features including keyboard/mouse input and OpenGL 
 * context setup.
 * 
 */
class PixelPipeWindow : public GlutWindow {
public:
	PixelPipeWindow(std::string title="PixelPipe", int width=800, int height=600, render_mode mode=RENDER_SOFTWARE);
	~PixelPipeWindow();
	virtual int run();
	
	/**
	 * Prepares the pipeline for rendering. The vertex and fragment processors are setup.
	 * The OpenGL context is initialized so that we can also draw the output.
	 */
	virtual void init();
	
	/**
	 * Output utility function for logging and debugging purposes.
	 */
	inline std::ostream& operator<<(std::ostream &out)
	{
		return out << "[ PixelPipeWindow ]";
	}
	
protected:	
	render_mode m_mode;		//!< the current mode the pipeline is rendering in
	Scene* m_scene;			//!< the current scene instance
	Camera* m_camera;		//!< the camera instance being used
	Pipeline* m_pipeline;	//!< the pipeline instance
	State* m_state;			//!< the global renderer state
	std::vector<Texture*> m_textures;	//!< the textures to be loaded from external files. 
	
	virtual int render();
	virtual int resize(int width, int height);
	virtual int keyPressed(unsigned char key);
	virtual int keyReleased(unsigned char key);
	virtual int mousePressed(int button, int x, int y);
	virtual int mouseReleased(int button, int x, int y);
	virtual int mouseMoved(int x, int y);

private:
	/**
	 * Render software mode
	 */
	void render_softwareMode();

	/**
	 * Render OpenGL mode
	 */
	void render_openGLMode();

	/**
	 * Render OpenGL mode
	 */
	void render_CUDAMode();
	
	/**
	 * Init software mode
	 */
	void init_softwareMode();

	/**
	 * Init OpenGL mode
	 */
	void init_openGLMode();

	/**
	 * Init OpenGL mode
	 */
	void init_CUDAMode();

};	// PixelPipeWindow

}	// namespace pipeline

#endif	// __PIPELINE_APP_WINDOW_H