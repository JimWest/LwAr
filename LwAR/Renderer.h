#pragma once

#include <functional>
#include "Text.h"
#include "Object3d.h"
#include "opencv2/opencv.hpp"
#include "Light.h"

namespace lwar
{
	class Renderer
	{
	public:
		Renderer() {};
		virtual ~Renderer() {};
		virtual void initObject(Object3d& object) {};
		virtual void preDraw() {};
		virtual void drawText(Text& text) {};
		virtual void drawObject(Object3d& object, glm::mat4& projectionMatrix, glm::mat4& viewMatrix, bool ignoreDepth) {};
		virtual void postDraw() {};
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
		std::function<void(int key)> onKeyboardInput;
		Light light;
	};
}
