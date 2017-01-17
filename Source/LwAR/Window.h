#pragma once

#include "ExternalLibs.h"
#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <Windows.h>
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include <chrono>
#include <ctime>

#include "OpenGLRenderer.h"
#include "Camera.h"
#include "Object3d.h"
#include "Scene.h"
#include "Text.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>

namespace lwar
{

	enum RenderType
	{
		OpenGL
	};

	class Window
	{

	public:
		std::function<void(lwar::Window& window, float deltaTime)> onUpdate;

		//Creates a new Window which can be used for 3d augmented reality.	
		Window();
		Window(int width, int height, std::string title, RenderType renderType = RenderType::OpenGL);
		~Window();

		// Addes a new Object3d to the window which will be rendered each frame. 
		// This needs to be called so it can be prepared for rendering.
		void addObject(Object3d &object);

		// Addes a text to the Window which will be rendered each frame.
		void addText(Text& text);

		// Starts the mainloop of the Window, only returns if the window was closed
		void start();
		void stop();
		Renderer* getRenderer() const;
		Scene& getScene();

		// Returns the last pressed key from the window
		// Returns -1, if no Key was pressed
		int getLastKey();

		void getWindowPosition(int* xPos, int* yPos);

		// Returns the current background of the window
		cv::Mat getBackground();

		// Sets the current background of the window
		void setBackground(cv::Mat background);

		// Returns the 2D Point of the screen of the given position 3D Position in the Scene	
		// Adapted from http://webglfactory.blogspot.de/2011/05/how-to-convert-world-to-screen.html
		glm::vec2 worldToScreenPoint(glm::vec3 position);

		// Returns the 3D Coordinates of the Object in the scene of the given 2D Point of the screen
		// Adapted from http://webglfactory.blogspot.de/2011/05/how-to-convert-world-to-screen.html
		glm::vec3 screenToWorldPoint(glm::vec2 position);
		float screenToWorldDistance(float distance);
		float worldToScreenDistance(float distance);

	private:

		int width = 640;
		int height = 480;
		float fov = 45.0f;
		float zNear = 0.1f;
		float zFar = 200.0f;
		float worldCameraDist = 3.0f;
		bool running = false;
		int lastKey = -1;

		Renderer* renderer;
		Scene scene;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		Object3d background = Object3d(Primitves::Quad);
		void onRendererKeyInput(int key);

	};

}

