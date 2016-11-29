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
#include "Object3d.h"

using namespace std;

int width = 640;
int height = 480;

// TODO: Auto copy needed dlls into the output folder

int main()
{

	bool running = true;
	cv::Mat camFrame;
	OpenGLRenderer renderer = OpenGLRenderer(width, height, "Augmented Reality Test");

	// Create a our video capture using the Kinect and OpenNI
	// Note: To use the cv::VideoCapture class you MUST link in the highgui lib (libopencv_highgui.so)
	cout << "Opening Webcam device ..." << endl;

	Camera cam = Camera(0, width, height, 60);
	if (!cam.isOpened)
	{
		cout << "Error openeing camera" << endl;
		//return -1;

		camFrame = cv::imread("lena1.png",1);
	}
	
	Object3d background = Object3d(Object3d::Quad);
	Object3d triangle = Object3d(Object3d::Triangle);

	renderer.PrepareObject(&background);
	renderer.PrepareObject(&triangle);

	while (!renderer.quit)
	{
		if (cam.isOpened)
		{
			camFrame = cam.Retrieve();		
			// mirror on y axis so its like a mirror
			cv::flip(camFrame, camFrame, 1);
		}

		renderer.PreDraw();
		
		renderer.DrawObject(&background, camFrame);
		renderer.DrawObject(&triangle, camFrame);		

		renderer.PostDraw();
	}

	return 0;
}

