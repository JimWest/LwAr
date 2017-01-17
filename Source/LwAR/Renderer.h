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
		// Inits an object, needs to be called before drawing it
		virtual void initObject(Object3d& object) {};		
		virtual void preDraw() {};
		// Renders the given Text on the scene
		virtual void drawText(Text& text) {};
		// Renders the given Object on the scene
		virtual void drawObject(Object3d& object, glm::mat4& projectionMatrix, glm::mat4& viewMatrix, bool ignoreDepth) {};
		// Post render operations like Buffer swapping. Needs to be called after the rendering.
		virtual void postDraw() {};
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
		virtual bool getShouldClose() = 0;
		std::function<void(int key)> onKeyboardInput;
		Light light;
	};
}
