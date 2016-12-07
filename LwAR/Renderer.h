#pragma once

#include "Object3d.h"
#include "opencv2/opencv.hpp"

class Renderer
{
public:	
	Renderer() {};
	virtual ~Renderer() {};
	virtual void initObject(Object3d& object) {};
	virtual void preDraw() {};
	virtual void drawObject(Object3d& object, bool ignoreDepth = false) {};
	virtual void postDraw() {};
};

