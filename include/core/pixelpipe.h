#ifndef __PIPELINE_APP_WINDOW_H
#define __PIPELINE_APP_WINDOW_H

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "core/glutwindow.h"
#include "core/camera.h"
#include "core/pipeline.h"
#include "core/geometry.h"
#include "core/scene.h"

namespace pipeline {
	
class PixelPipeWindow : public GlutWindow {
public:
	PixelPipeWindow(std::string title="PixelPipe", int width=800, int height=600);
	~PixelPipeWindow();
	virtual int run();
	virtual void init();
	
protected:	
	SceneCube* m_scene;
	Camera* m_camera;
	Pipeline* m_pipeline;
	
	virtual int render();
	virtual int resize(int width, int height);
	virtual int keyPressed(unsigned char key);
	virtual int keyReleased(unsigned char key);
	virtual int mousePressed(int button, int x, int y);
	virtual int mouseReleased(int button, int x, int y);
	virtual int mouseMoved(int x, int y);

private:


};	// PixelPipeWindow

}	// pipeline

#endif	// __PIPELINE_APP_WINDOW_H