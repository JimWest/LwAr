// LwAR.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>  
#include <stdlib.h>  
#include <Windows.h>
#include <iostream>

#include "OpenGLRenderer.h"
#include "Camera.h"
#include "LwAR.h"

using namespace std;


// TODO: Auto copy needed dlls into the output folder

int main()
{
	bool running = true;
	OpenGLRenderer renderer = OpenGLRenderer();

	// Create a our video capture using the Kinect and OpenNI
	// Note: To use the cv::VideoCapture class you MUST link in the highgui lib (libopencv_highgui.so)
	cout << "Opening Webcam device ..." << endl;

	Camera cam = Camera(0, 640, 480, 30);
	if (!cam.isOpened)
	{
		cout << "Error openeing camera" << endl;
		return -1;
	}

	// Create our cv::Mat objects
	cv::Mat camFrame;
	cv::Mat depthFrame;

	while (!renderer.quit)
	{
		camFrame = cam.Retrieve();
		renderer.Draw(camFrame);
	}

	return 0;
}

