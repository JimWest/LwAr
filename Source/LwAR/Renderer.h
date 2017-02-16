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

		// Initializes an Object (creating the needed Buffers).
		// This needs to be called once before drawing the object.
		//
		// @param object The object that should be initialized.
		virtual void initObject(Object3d& object) {};	
		
		// Clears an Object (deleting the Buffers).
		// This needs to be called before the renderer is deleted
		//
		// @param object The object that should be cleared.
		virtual void clearObject(Object3d& object) {};

		// Handles all stuff that needs to be done before actually render something
		// like clearing the window etc. This needs to be called before rendering 
		// objects once per frame.
		virtual void preDraw() {};

		// Renders the given Text on the scene
		//
		// @param text The text that should be drawn.
		virtual void drawText(Text& text) {};

		// Renders the given Object on the scene
		//
		// @param object The box object that should be rendered.
		// @param projectionMatrix The projection Matrix of the furstum
		// @param viewMatrix The view Matrix of the camera
		// @param ignoreDepth Should the object ignore the depth Buffer
		virtual void drawObject(Object3d& object, glm::mat4& projectionMatrix, glm::mat4& viewMatrix, bool ignoreDepth) {};

		// Handles the events that needs to be happen after the rendering.
		// This needs to be called after rendereing all the objects once per frame.
		virtual void postDraw() {};
		virtual int getWindowWidth() = 0;
		virtual int getWindowHeight() = 0;
		virtual void getWindowPosition(int* xPos, int* yPos) = 0;
		virtual bool getShouldClose() = 0;

		// Callback for Keyboard Input
		std::function<void(int key)> onKeyboardInput;
		Light light;
	};
}
