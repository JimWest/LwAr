#pragma once

// Add all needed librarys
#ifdef _DEBUG 
#pragma comment(lib, "opencv_calib3d2413d.lib")
#pragma comment(lib, "opencv_core2413d.lib")
#pragma comment(lib, "opencv_highgui2413d.lib")
#pragma comment(lib, "opencv_imgproc2413d.lib")
#else
#pragma comment(lib, "opencv_calib3d2413.lib")
#pragma comment(lib, "opencv_core2413.lib")
#pragma comment(lib, "opencv_highgui2413.lib")
#pragma comment(lib, "opencv_imgproc2413.lib")
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

#include "OpenGLRenderer.h"
#include "Camera.h"
#include "Object3d.h"

class LwAR
{

public:

	LwAR();
	LwAR(Renderer* renderer);
	~LwAR();
	void AddObject(Object3d &object);
	void Start();
	std::function<void()> onUpdate;

	Object3d background = Object3d(Primitves::Quad);

private:

	int _width = 640;
	int _height = 480;
	bool _running;
	Renderer* _renderer;
	cv::Mat gradientTexture;
	std::vector<Object3d*> _objects;
};
