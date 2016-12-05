// LwARTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../LwAR/LwAR.h"
#include "../LwAR/glm/glm.hpp"
#include "../LwAR/glm/gtc/matrix_transform.hpp"
#include "../LwAR/glm/gtc/quaternion.hpp"
#include "../LwAR/glm/gtx/quaternion.hpp"
#include "../LwAR/glm/gtx/euler_angles.hpp"


int width = 640;
int height = 480;
std::string windowName = "Augmented Reality Test";

void onUpdate()
{
	std::cout << "onUpdate" << '\n';
}

int main()
{
	LwAR lwar = LwAR();
	lwar.onUpdate = onUpdate;

	OpenGLRenderer renderer = OpenGLRenderer(width, height, windowName);

	// Create a our video capture using the Kinect and OpenNI
	// Note: To use the cv::VideoCapture class you MUST link in the highgui lib (libopencv_highgui.so)
	std::cout << "Opening Webcam device ..." << std::endl;

	Camera _cam = Camera(0, width, height, 60);
	if (!_cam.isOpened)
	{
		std::cout << "Error openeing camera" << std::endl;
		//return -1;
	}

	Object3d background = Object3d(Object3d::Quad);
	Object3d cube = Object3d(Object3d::Cube);

	renderer.initObject(&background);
	renderer.initObject(&cube);

	cube.transform.translation = glm::vec3(-0.2f, 0.1f, 0.1f);
	cube.transform.scale = glm::vec3(0.5f, 0.5f, 0.5f);
	cube.transform.rotation = glm::quat(glm::vec3(10, 10, 10));



    return 0;
}

