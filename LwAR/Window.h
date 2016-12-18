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
#include <chrono>
#include <ctime>

#include "gtc/matrix_transform.hpp"

#include "OpenGLRenderer.h"
#include "Camera.h"
#include "Object3d.h"
#include "Scene.h"
#include "vec3.hpp"

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

		Window();
		Window(int width, int height, std::string title, RenderType renderType = RenderType::OpenGL);
		~Window();
		void addObject(Object3d &object);
		void start();
		void stop();

		Renderer* getRenderer() const;
		Scene& getScene();
		int getLastKey();
		cv::Mat getBackground();
		void setBackground(cv::Mat background);

		glm::vec2 worldToScreenPoint(glm::vec3 position);
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
		int lastKey = 0;

		Renderer* renderer;
		Scene scene;
		glm::mat4 projectionMatrix;
		glm::mat4 viewMatrix;
		Object3d background = Object3d(Primitves::Quad);
		void onRendererKeyInput(int key);

	};

}

