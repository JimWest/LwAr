#pragma once

#include "Object3d.h"
#include "opencv2/opencv.hpp"

namespace lwar
{
	class Renderer
	{
	public:
		Renderer() {};
		virtual ~Renderer() {};
		virtual void initObject(Object3d& object) {};
		virtual void preDraw() {};
		virtual void drawObject(Object3d& object, bool ignoreDepth = false) {};
		virtual void postDraw() {};
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
	};
}
