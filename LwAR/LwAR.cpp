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
	Object3d cube = Object3d(Object3d::Cube);

	renderer.initObject(&background);
	renderer.initObject(&cube);
	
	cube.transform.translation = glm::vec3(-0.2f, 0.1f, 0.1f);
	cube.transform.scale = glm::vec3(0.1f, 0.1f, 0.1f);
	cube.transform.rotation = glm::quat(glm::vec3(10, 10, 10));

	while (!renderer.quit)
	{
		if (cam.isOpened)
		{
			camFrame = cam.Retrieve();		
			// mirror on y axis so its like a mirror
			cv::flip(camFrame, camFrame, 1);
		}

		renderer.preDraw();
		
		renderer.drawObject(&background, camFrame);

		renderer.drawObject(&cube, camFrame);

		renderer.postDraw();
	}

	return 0;
}



