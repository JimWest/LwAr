// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/LwAR.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/quaternion.hpp"
#include "gtx/quaternion.hpp"
#include "gtx/euler_angles.hpp"


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";


Object3d background;
Object3d cube;
Camera cam;
cv::Mat camFrame;

int i = 0;

void onUpdate()
{
	if (cam.isOpened)
	{
		camFrame = cam.Retrieve();
		// mirror on y axis so its like a mirror
		cv::flip(camFrame, camFrame, 1);
	}

	background.material.texture = camFrame;
	cube.transform.rotation = glm::quat(glm::vec3(10, 1 * i / 100.0f, 1 * i / 100.0f));
	i++;
}

int main()
{
	OpenGLRenderer renderer = OpenGLRenderer(width, height, windowName);

	LwAR lwar = LwAR(&renderer);
	lwar.onUpdate = onUpdate;

	// Create a our video capture using the Kinect and OpenNI
	// Note: To use the cv::VideoCapture class you MUST link in the highgui lib (libopencv_highgui.so)
	std::cout << "Opening Webcam device ..." << std::endl;

	cam = Camera(0, width, height, 60);
	cam.init();
	if (!cam.isOpened)
	{
		std::cout << "Error openeing camera" << std::endl;
		//return -1;
	}

	background = Object3d(Primitves::Quad);
	cube = Object3d(Primitves::Cube);
	
	cube.transform.translation = glm::vec3(-0.2f, 0.1f, 0.1f);
	cube.transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	cube.transform.rotation = glm::quat(glm::vec3(10, 10, 10));

	lwar.AddObject(background);
	lwar.AddObject(cube);

	lwar.Start();

    return 0;
}

