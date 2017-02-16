#include "stdafx.h"
#include "Window.h"
#include "glm.hpp"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";
lwar::Camera camera;

void onUpdate(lwar::Window& window, float deltaTime);

int main()
{
	lwar::Window window;
	window.onUpdate = onUpdate;

	window.create(width, height, windowName);

	camera = lwar::Camera(0, width, height);
	camera.init();

	if (!camera.isOpened)
	{
		getchar();
		return -1;
	}

	// starts the main loop
	window.start();

	return 0;
}


void onUpdate(lwar::Window& window, float deltaTime)
{
	cv::Mat camFrame;

	if (camera.isOpened)
	{
		camFrame = camera.retrieve();
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