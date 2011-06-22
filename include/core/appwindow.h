#ifndef __PIPELINE_APP_WINDOW_H
#define __PIPELINE_APP_WINDOW_H

/**
 * Copyright (C) 2011 by Implied Sciences
 *
 * This file is part of Physical Vision.
 *
 */

#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "core/glutwindow.h"

namespace pipeline {
	
class ApplicationWindow : public GlutWindow {
public:
	ApplicationWindow(std::string title="Application Window", int width=800, int height=600);
	~ApplicationWindow();
	virtual int run();
	virtual void init();
	
protected:
	virtual int render();
	virtual int resize(int width, int height);
	virtual int keyPressed(unsigned char key);
	virtual int keyReleased(unsigned char key);
	virtual int mousePressed(int button, int x, int y);
	virtual int mouseReleased(int button, int x, int y);
	virtual int mouseMoved(int x, int y);

private:


};	// ApplicationWindow

}	// pipeline

#endif	// __PIPELINE_APP_WINDOW_H