#pragma once

#include <functional>
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
		virtual void drawText(const char* text, int x, int y, int size) {};
		virtual void drawObject(Object3d& object, glm::mat4& projectionMatrix, glm::mat4& viewMatrix, bool ignoreDepth) {};
		virtual void postDraw() {};
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
		std::function<void(int key)> onKeyboardInput;
	};
}
