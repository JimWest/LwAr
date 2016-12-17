// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>

int width = 640;
int height = 480;
std::string windowName = "Lightweight Augmented Reality Framework Test";

void onUpdate(lwar::Window& window)
{
	lwar::Renderer* renderer = window.getRenderer();
	lwar::Scene& scene = window.getScene();
	cv::Mat camFrame;

	if (scene.camera.isOpened)
	{
		camFrame = scene.camera.retrieve();
		// mirror on y axis so its like a mirror
		cv::flip(camFrame, camFrame, 1);
	}

	// set the background of the window to the current camera image
	window.setBackground(camFrame);

	// check for keys
	int key = window.getLastKey();

	// escape == exit
	if (key == 256)
		window.stop();
}

int main()
{
	lwar::Window window = lwar::Window(width, height, windowName);
	window.onUpdate = onUpdate;

	lwar::Camera camera = lwar::Camera(0, width, height, 60);
	camera.init();

	if (!camera.isOpened)
	{
		return -1;
	}

	window.getScene().camera = camera;

	// starts the main loop
	window.start();

	return 0;
}

