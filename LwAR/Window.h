#pragma once

// Add all needed librarys
#ifdef _DEBUG 
#pragma comment(lib, "opencv_calib3d2413d.lib")
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#pragma comment(lib, "opencv_objdetect2413d.lib")

#else
#pragma comment(lib, "opencv_calib3d2413.lib")
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
#pragma comment(lib, "opencv_objdetect2413.lib")
#endif

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <Windows.h>
#include <iostream>
#include <functional>
#include <vector>
#include <memory>
#include "gtc/matrix_transform.hpp"

#include "OpenGLRenderer.h"
#include "Camera.h"
#include "Object3d.h"
#include "Scene.h"

// TODO: TIME, KEYS/ MOUSE

namespace lwar
{

	enum RenderType
	{
		OpenGL
	};

	class Window
	{

	public:

		Window();
		Window(int width, int height, std::string title, RenderType renderType = RenderType::OpenGL);
		~Window();
		void addObject(Object3d &object);
		void start();
		void stop();

		void onRendererKeyInput(int key);

		Renderer* getRenderer() const;
		Scene& getScene();
		int getLastKey();

		glm::vec2 worldToScreenPoint(glm::vec3 position);
		glm::vec3 screenToWorldPoint(glm::vec2 position);

		float screenToWorldRadius(float radius);
		float worldToScreenRadius(float radius);

		//glm::vec2 Window::worldToScreenScale(glm::vec3 scale);
		//glm::vec3 Window::screenToWorldScale(glm::vec2 scale);

		std::function<void(lwar::Window& window, int key, int scancode, int action, int mods)> onKeyboardInput;
		std::function<void(lwar::Window& window)> onUpdate;
		Object3d background = Object3d(Primitves::Quad);

	private:

		int _width = 640;
		int _height = 480;
		bool _running = false;
		int _lastKey = 0;
		Renderer* _renderer;
		Scene _scene;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;

	};
}

